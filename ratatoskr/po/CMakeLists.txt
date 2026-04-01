include(FindGettext)
find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext)

set(DOMAIN ${FULL_PROJECT_NAME})
set(POT_FILE ${DOMAIN}.pot)
file(GLOB PO_FILES *.po)

# Creates the .pot file containing the translations template
add_custom_target(${POT_FILE} ALL
    COMMENT "Generating translation template"
    # Pass 1: Extract translatable strings from the desktop file
    COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} -o ${POT_FILE}
        --from-code=UTF-8
        --language=Desktop
        -k -kName -kComment -kGenericName
        --package-name='${DOMAIN}'
        --sort-by-file
        -D ${CMAKE_SOURCE_DIR}
        ${DESKTOP_FILE_NAME}.in

    # Pass 2: Extract translatable strings from QML and C++ source files
    COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} -o ${POT_FILE}
        --from-code=UTF-8
        --c++ --qt --add-comments=TRANSLATORS
        --keyword=tr --keyword=tr:1,2 --keyword=ctr:1c,2 --keyword=dctr:2c,3 --keyword=N_ --keyword=_
        --keyword=dtr:2 --keyword=dtr:2,3 --keyword=tag --keyword=tag:1c,2
        --package-name='${DOMAIN}'
        --sort-by-file
        --join-existing
        -D ${CMAKE_SOURCE_DIR}
        ${I18N_SRC_FILES}
    COMMAND ${CMAKE_COMMAND} -E copy ${POT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})

# Builds the binary translations catalog for each language
# it finds source translations (*.po) for
foreach(PO_FILE ${PO_FILES})
    get_filename_component(LANG ${PO_FILE} NAME_WE)
    gettext_process_po_files(${LANG} ALL PO_FILES ${PO_FILE})
    set(INSTALL_DIR ${CMAKE_INSTALL_LOCALEDIR}/share/locale/${LANG}/LC_MESSAGES)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${LANG}.gmo
            DESTINATION ${INSTALL_DIR}
            RENAME ${DOMAIN}.mo)
endforeach(PO_FILE)
