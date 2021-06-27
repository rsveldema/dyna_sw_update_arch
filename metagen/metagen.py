import sys
import re
from typing import ForwardRef
from jinja2 import Template


class Field:
    name: str
    
    def __init__(self, name: str, type: str):
        self.name = name
        self.type = type
    
    def dump(self):
        print("\t" + self.name)


class Clazz:
    name: str
    compat_clazz: 'Clazz'
    fields: 'list[Field]'
    def __init__(self, file: str, name: str, base_class: str, compat_clazz: 'Clazz', namespace: 'Namespace'):
        self.file = file
        self.name = name
        self.fields = []
        self.compat_clazz = compat_clazz
        self.base_class = base_class
        self.namespace = namespace

    def make_compatible(self):
        if self.compat_clazz == None:
            return
        for f in self.compat_clazz.fields:
            self.removeField(f.name)

        new_field_list = []
        for f in self.compat_clazz.fields:
            new_field_list.append(f)

        for f in self.fields:
            new_field_list.append(f)
        
        self.fields = new_field_list

    def removeField(self, fieldName):
        for f in self.fields:
            if f.name == fieldName:
                self.fields.remove(f)                
                return

    def dump(self):
        print("CLASS: " + self.name)
        for member in self.fields:
            member.dump()

    def fqn(self):
        me = str(len(self.name)) + self.name + "E"
        return self.namespace.fqn() + me
    
    def add(self, member):
        member.first = len(self.fields) == 0
        self.fields.append(member)

class Namespace:
    name: str
    class_map:'dict[str, Clazz]' = {}
    nested_namespaces: 'dict[str, Namespace]' = {}
    parent: 'Namespace'

    def __init__(self, name, scope_level, parent):
        self.name = name
        self.scope_level = scope_level
        self.parent = parent

    def lookup(self, class_name) -> Clazz:
        if class_name in self.class_map:
            return self.class_map[class_name]
        if self.parent != None:
            return self.parent.lookup(class_name)   
        return None

    def fqn(self):
        me = "N" + str(len(self.name)) + self.name
        if self.parent != None:
            return self.parent.fqn() + me
        return ""

    def dump_classes(self):
        for clazz in self.class_map:
            self.class_map[clazz].dump()

    def dump(self):
        self.dump_classes()

    def add(self, clazz: Clazz):
        self.class_map[clazz.name] = clazz

    def extend_class_list(self, classes):
        for c in self.class_map:
            clazz = self.class_map[c]
            classes.append(clazz)
        for nested in self.nested_namespaces:
            nested.extend_class_list(classes)

    def get_class_list(self):
        classes = []
        self.extend_class_list(classes)
        return classes

    def make_compat(self):
        for c in self.class_map:
            clazz = self.class_map[c]
            clazz.make_compatible()
        for nested in self.nested_namespaces:
            nested.make_compat()


root_namespace: Namespace = Namespace("", 0, None)

current_namespace = root_namespace


def tokenize_string(line: str):
    str = line.strip()
    tokenized = re.split('([,;:\<\>\[\]\{\}\(\)\s])',str)

    output = []
    for t in tokenized:
        if len(t.strip()) > 0:
            if re.match("^\d+?\.\d+?$",t) is None:
                if re.match("^[\d\-]\d*?$",t) is None:
                    output.append(t)
                else:
                    output.append(int(t))
            else:
                output.append(float(t))

    return output


def generate_metadata(output_filename, template_filename):
    fp = open(template_filename, "r")
    data = fp.read()
    fp.close()
    tm = Template(data)

    classes = root_namespace.get_class_list()

    fp = open(output_filename, "w")
    new_data = tm.render(classes=classes)
    fp.write(new_data)
    fp.close()

class Position:
    def __init__(self, filename: str):
        self.filename = filename
        self.line = 1

    def error(self, msg):
        print(self.filename + ":" + self.line + ": " + msg)
    
def parse_file(filename: str):
    global current_namespace
    fp = open(filename, "r")
    lines = fp.readlines()
    fp.close()

    scope_level = 0

    lineNo = 0
    pos = Position(filename)
    current_class = None
    for line in lines:
        toks = tokenize_string(line)
        pos.line = lineNo
        lineNo += 1

        # ['class', 'MyData_V1', ':', 'public', 'UpdateableObject']
        # ['class', 'MyData_V3', ':', 'public', 'UpdatedObject', '<', 'MyData_V1', '>']
        # ['FIELD', '(', 'int,', 'd2,', 5678, ')', ';']

        #print("TOKS = " + str(toks))
        if len(toks) == 0:
            pass
        elif toks[0] == '{':
            scope_level += 1
        elif toks[0] == '}':
            scope_level -= 1
            if current_namespace.scope_level == scope_level:
                current_namespace = current_namespace.parent
        elif toks[0].startswith("namespace"):
            current_namespace = Namespace(toks[1], scope_level, current_namespace)
        elif toks[0].startswith("class"):
            if len(toks) > 3:
                name = toks[1]
                base_class = toks[4]

                compat_clazz = None
                
                if len(toks) > 6:
                    compat_class_name = toks[6]   
                    compat_clazz = current_namespace.lookup(compat_class_name)
                    if compat_clazz == None:
                        pos.error("failed to find compat class " + compat_class_name)

                current_class = Clazz(filename, name, base_class, compat_clazz, current_namespace)     
                current_namespace.add(current_class)
            else:
                current_class = None
        elif toks[0].startswith("FIELD") and current_class != None:
            type = toks[2]
            name = toks[4]
            member = Field(name, type)
            current_class.add(member)


output_filename = sys.argv[1]
template_filename = sys.argv[2]
input_filename = sys.argv[3]

parse_file(input_filename)

root_namespace.dump()
root_namespace.make_compat()

generate_metadata(output_filename, template_filename)