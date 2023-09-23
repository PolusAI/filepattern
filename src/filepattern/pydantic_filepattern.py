# -*- coding: utf-8 -*-
from pydantic import create_model


def get_pydantic_fp(file: tuple):
    # get variable names and types for pydantic class
    variable_type_map = {}

    # add paths to map
    variable_type_map["path"] = (list, ...)

    # get variables mapped to values
    variables = file[0]

    # loop over variables and get type
    for variable in variables:
        variable_type_map[variable] = (type(variables[variable]), ...)

    return create_pydantic_fp(variable_type_map)


def create_pydantic_fp(file_dict: dict):

    return create_model(
        'FilepatternModel',
        **file_dict
    )
