# -*- coding: utf-8 -*-

"""
This module is the bridge between all type modules defined in giwtypes and the
debugger. For every symbol found by the debugger, this module queries for a
candidate giwtype and forwards the required calls from the debugger to it.
"""

import importlib
import pkgutil

from giwscripts import giwtypes
from giwscripts.giwtypes.interface import TypeInspectorInterface


class TypeBridge:
    """
    Class responsible for loading and interfacing with all modules implementing
    the TypeInspectorInterface
    """
    def __init__(self):
        self._type_inspectors = []

        # Import all modules within giwtypes
        for (_, mod_name, _) in pkgutil.iter_modules(giwtypes.__path__):
            importlib.import_module('.giwtypes.' + mod_name, __package__)

        # Save instances of all TypeInspector implementations
        for inspector_class in TypeInspectorInterface.__subclasses__():
            self._type_inspectors.append(inspector_class())

    def get_buffer_metadata(self, symbol_name, picked_obj, debugger_bridge):
        """
        Returns the metadata related to a variable, which are required for the
        purpose of plotting it in the giwwindow
        """
        for module in self._type_inspectors:
            if module.is_symbol_observable(picked_obj, symbol_name):
                return module.get_buffer_metadata(symbol_name,
                                                  picked_obj,
                                                  debugger_bridge)

        return None

    def is_symbol_observable(self, symbol_obj, symbol_name):
        """
        Returns true if any available module is able to process this particular
        symbol
        """
        for module in self._type_inspectors:
            if module.is_symbol_observable(symbol_obj, symbol_name):
                return True

        return False
