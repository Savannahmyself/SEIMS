#! /usr/bin/env python
# coding=utf-8

from xml.etree.ElementTree import ElementTree,Element
from xml.dom.minidom import Document
import os
import linecache
import re
import glob

def readFileToDict(filename):
    var = []
    var_type = []
    var_name = []
    var_description = []
    var_unit = []
    var_dimension = []
    var_source = []

    text = ["@In","@Out","@parameter"]

    file = open(filename)
    line = file.readline()
    line_num = 1

    while line:
        for i in range(len(text)):
            bool = line.find(text[i])
            if bool > 0:
                break

        if bool > 0:
            str_des = linecache.getline(filename, (line_num + 1)).strip()
            str_var = linecache.getline(filename, (line_num + 3)).strip()
            type = re.split('@|\n', line)
            des = str_des.replace('// @Description ','').replace('\n','')
            name = str_var.replace("*", "").replace(";", "").replace(",", "").split()
            dimension = str_var.replace(";","").replace(",","").split()

            str_source = linecache.getline(filename, (line_num + 2)).strip()
            print (str_source)
            source = str_source.replace('// @Source ','').replace('\n','')
            print (source)

            dim = dimension[0]
            for i in range(1,len(name)):
                if "*" in dimension[i]:
                    dim = dimension[0] + " *"
                if "**" in dimension[i]:
                    dim = dimension[0] + " **"
                var_dimension.append(dim)
                var_description.append(des)
                var_type.append(type[1])
                var_name.append(name[i])
                var_source.append(source)
        line_num += 1
        line = file.readline()

    for i in range(len(var_name)):
        var_dic = {}
        var_dic["type"] = var_type[i].lower()
        var_dic["name"] = var_name[i]
        var_dic["description"] = var_description[i]
        var_dic["dimension"] = var_dimension[i]
        var_dic["source"] = var_source[i]
    # print var_dic
        var.append(var_dic)
    # print var
    file.close()

    return var

def createXML(xml_name,var):

    doc = Document()  # 创建DOM文档对象
    metadata = doc.createElement('metadata')  # 创建根元素metadata
    doc.appendChild(metadata)
    metadata.setAttribute("version","4.0")

    type = []
    for i in range(len(var)):
        var_dic = var[i]
        type.append(var_dic["type"])
    type = list(set(type))
    print(type)
    if "in" in type:
        inputs = doc.createElement('inputs')  # 创建metadata下第一节点inputs
        metadata.appendChild(inputs)
    if "parameter" in type:
        parameters = doc.createElement('parameters')  # 创建metadata下第一节点parameters
        metadata.appendChild(parameters)
    if "out" in type:
        outputs = doc.createElement('outputs')  # 创建metadata下第一节点outputs
        metadata.appendChild(outputs)

    for i in range(len(var)):
        var_dic = var[i]
        if (var_dic["type"] == "in"):
            inputvar = doc.createElement('inputvariables')
            inputs.appendChild(inputvar)

            name = doc.createElement("name")
            name.appendChild(doc.createTextNode(var_dic["name"]))
            description = doc.createElement("description")
            description.appendChild(doc.createTextNode(var_dic["description"]))
            # unit = doc.createElement("unit")
            # unit.appendChild(doc.createTextNode(var_dic["unit"]))
            dimension = doc.createElement("dimension")
            dimension.appendChild(doc.createTextNode(var_dic["dimension"]))
            source = doc.createElement("source")
            source.appendChild(doc.createTextNode(var_dic["source"]))
            inputvar.appendChild(name)
            inputvar.appendChild(description)
            # inputvar.appendChild(unit)
            inputvar.appendChild(dimension)
            inputvar.appendChild(source)

        if (var_dic["type"] == "parameter"):
            parameter = doc.createElement('parameter')
            parameters.appendChild(parameter)

            name = doc.createElement("name")
            name.appendChild(doc.createTextNode(var_dic["name"]))
            description = doc.createElement("description")
            description.appendChild(doc.createTextNode(var_dic["description"]))
            # unit = doc.createElement("unit")
            # unit.appendChild(doc.createTextNode(var_dic["unit"]))
            dimension = doc.createElement("dimension")
            dimension.appendChild(doc.createTextNode(var_dic["dimension"]))
            source = doc.createElement("source")
            source.appendChild(doc.createTextNode(var_dic["source"]))
            parameter.appendChild(name)
            parameter.appendChild(description)
            # parameter.appendChild(unit)
            parameter.appendChild(dimension)
            parameter.appendChild(source)

        if (var_dic["type"] == "out"):
            outputvar = doc.createElement('outputvariables')
            outputs.appendChild(outputvar)

            name = doc.createElement("name")
            name.appendChild(doc.createTextNode(var_dic["name"]))
            description = doc.createElement("description")
            description.appendChild(doc.createTextNode(var_dic["description"]))
            # unit = doc.createElement("unit")
            # unit.appendChild(doc.createTextNode(var_dic["unit"]))
            dimension = doc.createElement("dimension")
            dimension.appendChild(doc.createTextNode(var_dic["dimension"]))
            source = doc.createElement("source")
            source.appendChild(doc.createTextNode(var_dic["source"]))
            outputvar.appendChild(name)
            outputvar.appendChild(description)
            # outputvar.appendChild(unit)
            outputvar.appendChild(dimension)
            outputvar.appendChild(source)

    # print doc.toprettyxml(indent=" ")

    with open(xml_name + '.xml', "w") as f:
        f.write(doc.toprettyxml(indent=" "))
    print("create xml successfully")

if __name__ == '__main__':

    filelist = ["clsTSD_RD.h"]
    xml_namelist = ["TSD_RD"]
    
    n = len(filelist)
    for i in range(n):
        varlist = readFileToDict(filelist[i])
        createXML(xml_namelist[i], varlist)

