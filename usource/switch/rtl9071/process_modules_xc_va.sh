#!/bin/bash

# Define dependencies for each module
declare -A DEPENDENCIES_XC_VA=(
    ["tbl-access"]="ethernet-access"
    ["l2"]="tbl-access vlan"
    ["ptp"]="ethernet-access acl vlan spi-flash system port-isolation otp reset serdes interface phy-port"
    ["vlan"]=""
    ["dot1x"]="ethernet-access vlan"
    ["cb-frer"]="ethernet-access vlan routing tbl-access l2 system serdes otp phy-port reset tcam"
    ["stacking"]="ethernet-access system port-isolation tsn reset otp spi-flash phy-port serdes interface psfp"
    ["tsn"]="vlan ptp qos ethernet-access acl psfp spi-flash system port-isolation otp reset serdes interface phy-port"
    ["acl"]="tcam"
    ["tcam"]=""
    ["opfsm"]="phy-port"
    ["ethernet-access"]="spi-flash system port-isolation otp phy-port serdes interface reset sys-boot"
    ["port-mirror"]="tbl-access l2 vlan acl ethernet-access"
    ["stp"]=""
    ["single-loop"]="spi-flash reset sys-boot"
    ["qos"]="vlan qav"
    ["qav"]="qos vlan"
    ["st-qbv"]="ethernet-access tsn vlan ptp qos acl psfp spi-flash system port-isolation otp reset serdes interface phy-port"
    ["psfp"]="ethernet-access tsn vlan ptp qos acl st-qbv cb-frer routing tbl-access l2 spi-flash system port-isolation otp reset serdes interface phy-port"
    ["pcie"]="system ethernet-access port-isolation otp reset serdes phy-port interface"
    ["port-isolation"]="ethernet-access system otp reset serdes phy-port interface"
    ["storm-ctrl"]=""
    ["link-aggregation"]=""
    ["mib"]=""
    ["interface"]="phy-port serdes"
    ["smi-master"]="ethernet-access"
    ["spi-flash"]="ethernet-access otp"
    ["system"]="otp"
    ["phy-port"]="sys-boot"
    ["phy-test-mode"]="system ethernet-access otp serdes port-isolation interface phy-port tcam"
    ["flowctrl"]="vlan"
    ["igmp"]="vlan ethernet-access"
    ["otp"]=""
    ["reset"]=""
    ["sys-port"]="ethernet-access"
    ["sys-boot"]="ethernet-access phy-port opfsm interface otp serdes reset tcam"
    ["serdes"]="interface system"
    ["macphy"]=""
    ["macsec"]=""
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
    for dep in ${DEPENDENCIES_XC_VA[$module]}; do
        if [[ ! " ${PROCESSED_MODULES[@]} " =~ " ${dep} " ]]; then
            PROCESSED_MODULES+=("$dep")
            CFLAGS+=("-D$(add_prefix "$dep")")
            find_dependencies "$dep"
        fi
    done
}
echo "" > makefile.rtl907xc_va_modules
# Read modules from input arguments
for module in "${!DEPENDENCIES_XC_VA[@]}"; do
    CFLAGS=()
    PROCESSED_MODULES=()

    echo $module
    # Add the module's own CFLAGS first
    CFLAGS+=("-D$(add_prefix "$module")")
    PROCESSED_MODULES+=("$module")
    find_dependencies "$module"
    echo "ifneq (,\$(findstring $module, \$(RTL906X_MODULES)))" >> makefile.rtl907xc_va_modules
    #echo "MODULE_CFLAGS+=${CFLAGS[@]}" >> makefile.rtl907xc_va_modules
    echo "CFLAGS+=${CFLAGS[@]}" >> makefile.rtl907xc_va_modules
    echo "endif" >> makefile.rtl907xc_va_modules
done