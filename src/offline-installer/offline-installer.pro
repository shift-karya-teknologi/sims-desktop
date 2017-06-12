TEMPLATE = aux
INSTALLER = $$PWD/../../dist/ShiftIMS2017
INPUT = $$PWD/config/config.xml $$PWD/packages

extras.input = INPUT
extras.output = $$INSTALLER
extras.commands = ../../bin/binarycreator --offline-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
extras.CONFIG += target_predeps no_link combine

QMAKE_EXTRA_COMPILERS += extras

