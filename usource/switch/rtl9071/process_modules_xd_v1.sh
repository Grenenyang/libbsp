#!/bin/bash

# Define dependencies for each module
declare -A DEPENDENCIES_XD_V1=(
    ["tbl-access"]=""
    ["l2"]="tbl-access vlan"
    ["ptp"]="acl vlan"
    ["vlan"]=""
    ["stacking"]="tsn otp serdes phy-port phy-test-mode ethernet-access sys-boot"
    ["routing"]="l2"
    ["tsn"]=""
    ["acl"]=""
    ["opfsm"]="phy-port"
    ["ethernet-access"]=""
    ["port-mirror"]=""
    ["stp"]=""
    ["single-loop"]="reset spi-flash"
    ["qos"]="vlan"
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
    ["phy-test-mode"]="serdes otp phy-port ethernet-access interface sys-boot"
    ["flowctrl"]="vlan"
    ["igmp"]="vlan"
    ["otp"]=""
    ["reset"]=""
    ["sys-port"]=""
    ["sys-boot"]="reset interface serdes otp phy-port phy-test-mode ethernet-access opfsm"
    ["serdes"]="interface system"
    ["macphy"]=""
    ["macsec"]=""
    ["phy-port"]="sys-boot"
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
    for dep in ${DEPENDENCIES_XD_V1[$module]}; do
        if [[ ! " ${PROCESSED_MODULES[@]} " =~ " ${dep} " ]]; then
            PROCESSED_MODULES+=("$dep")
            CFLAGS+=("-D$(add_prefix "$dep")")
            find_dependencies "$dep"
        fi
    done
}
echo "" > makefile.rtl907xd_v1_modules
# Read modules from input arguments
for module in "${!DEPENDENCIES_XD_V1[@]}"; do
    CFLAGS=()
    PROCESSED_MODULES=()

    echo $module
    # Add the module's own CFLAGS first
    CFLAGS+=("-D$(add_prefix "$module")")
    PROCESSED_MODULES+=("$module")
    find_dependencies "$module"
    echo "ifneq (,\$(findstring $module, \$(RTL906X_MODULES)))" >> makefile.rtl907xd_v1_modules
    #echo "MODULE_CFLAGS+=${CFLAGS[@]}" >> makefile.rtl907xd_v1_modules
    echo "CFLAGS+=${CFLAGS[@]}" >> makefile.rtl907xd_v1_modules
    echo "endif" >> makefile.rtl907xd_v1_modules
done