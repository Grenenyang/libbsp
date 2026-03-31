#!/bin/bash

# Define dependencies for each module
declare -A DEPENDENCIES_XD_VA=(
    ["tbl-access"]=""
    ["l2"]="tbl-access vlan acl"
    ["ptp"]="acl vlan"
    ["vlan"]=""
    ["stacking"]="tsn otp serdes otp phy-port phy-test-mode ethernet-access sys-boot"
    ["routing"]="l2 acl tcam"
    ["tsn"]=""
    ["acl"]="tcam"
    ["tcam"]="acl"
    ["opfsm"]="phy-port"
    ["ethernet-access"]=""
    ["port-mirror"]=""
    ["stp"]="acl"
    ["single-loop"]="reset spi-flash"
    ["qos"]="vlan qav"
    ["qav"]="qos vlan"
    ["pcie"]="otp"
    ["port-isolation"]="interface serdes otp phy-port phy-test-mode ethernet-access"
    ["storm-ctrl"]=""
    ["link-aggregation"]=""
    ["mib"]=""
    ["interface"]="serdes phy-port"
    ["smi-master"]=""
    ["spi-flash"]="otp ethernet-access"
    ["system"]="otp"
    ["phy-test-mode"]="serdes otp phy-port ethernet-access interface"
    ["flowctrl"]="vlan"
    ["igmp"]="vlan acl"
    ["otp"]=""
    ["reset"]=""
    ["sys-port"]=""
    ["sys-boot"]="reset interface otp ethernet-access"
    ["serdes"]="interface system"
    ["macphy"]=""
    ["macsec"]="acl"
    ["phy-port"]="sys-boot"
    ["psfp"]="tsn ptp qos vlan cb-frer tbl-access"
    ["st-qbv"]="tsn qos ptp tbl-access"
    ["dot1x"]="acl vlan"
    ["cb-frer"]="routing tbl-access acl vlan tcam"
    ["ats"]="qos otp"
    ["firewall"]="routing"
    ["eee"]="phy-port serdes"
    ["someip"]=""
)

# Convert dependencies to uppercase and add prefix
to_uppercase() {
    echo "$1" | tr 'a-z-' 'A-Z_'
}

add_prefix() {
    echo "RTK_MODULE_$(to_uppercase "$1")"
}

# Recursively find dependencies and add to CFLAGS, avoiding duplicates and infinite recursion
find_dependencies() {
    local module="$1"
    for dep in ${DEPENDENCIES_XD_VA[$module]}; do
        if [[ ! " ${PROCESSED_MODULES[@]} " =~ " ${dep} " ]]; then
            PROCESSED_MODULES+=("$dep")
            CFLAGS+=("-D$(add_prefix "$dep")")
            find_dependencies "$dep"
        fi
    done
}
echo "" > makefile.rtl907xd_va_modules
# Read modules from input arguments
for module in "${!DEPENDENCIES_XD_VA[@]}"; do
    CFLAGS=()
    PROCESSED_MODULES=()

    echo $module
    # Add the module's own CFLAGS first
    CFLAGS+=("-D$(add_prefix "$module")")
    PROCESSED_MODULES+=("$module")
    find_dependencies "$module"
    echo "ifneq (,\$(findstring $module, \$(RTL906X_MODULES)))" >> makefile.rtl907xd_va_modules
    #echo "MODULE_CFLAGS+=${CFLAGS[@]}" >> makefile.rtl907xd_va_modules
    echo "CFLAGS+=${CFLAGS[@]}" >> makefile.rtl907xd_va_modules
    echo "endif" >> makefile.rtl907xd_va_modules
done