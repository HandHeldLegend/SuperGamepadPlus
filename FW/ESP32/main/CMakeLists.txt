#set(COMPONENT_SRCS "switch_analog.c" "switch_spi.c" "switch_commands.c" "core_bt_switch.c" "main.c")
#set(COMPONENT_ADD_INCLUDEDIRS "")

#register_component()

idf_component_register(SRCS "main.c" "core_bt_xinput.c" "esp_hid_gap.c" "util_bt_hid.c" "rsc_descriptors.c"
                    INCLUDE_DIRS "." "include" "include/switch")
