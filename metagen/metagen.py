import sys
import re
from typing import ForwardRef
from jinja2 import Template



class Member:
    name: str
    
    def __init__(self, name: str):
        self.name = name
    
    def dump(self):
        print("\t" + self.name)


class Clazz:
    name: str
    compat: str
    def __init__(self, name: str, base_class: str, compat: str, namespace: 'Namespace'):
        self.name = name
        self.fields = []
        self.compat = compat
        self.base_class = base_class
        self.namespace = namespace
    
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
    
def parse_file(filename: str):
    global current_namespace
    fp = open(filename, "r")
    lines = fp.readlines()
    fp.close()

    scope_level = 0

    current_class = None
    for line in lines:
        toks = tokenize_string(line)

        # ['class', 'MyData_V1', ':', 'public', 'UpdateableObject']
        # ['class', 'MyData_V3', ':', 'public', 'UpdatedObject', '<', 'MyData_V1', '>']
        # ['FIELD', '(', 'int,', 'd2,', 5678, ')', ';']

        print("TOKS = " + str(toks))
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
                
                if len(toks) > 6:
                    compat = toks[6]  
                else:
                    compat = ""
                current_class = Clazz(name, base_class, compat, current_namespace)         
                current_namespace.add(current_class)
            else:
                current_class = None
        elif toks[0].startswith("FIELD") and current_class != None:
            name = toks[4]
            member = Member(name)
            current_class.add(member)


output_filename = sys.argv[1]
template_filename = sys.argv[2]
input_filename = sys.argv[3]

parse_file(input_filename)

root_namespace.dump()

generate_metadata(output_filename, template_filename)