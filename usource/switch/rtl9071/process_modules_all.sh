#!/bin/bash
echo "Process XC_VA dependency"
source process_modules_xc_va.sh
echo "Process XC_VB dependency"
source process_modules_xc_vb.sh
echo "Process XD_V1 dependency"
source process_modules_xd_v1.sh
echo "Process XD_VA dependency"
source process_modules_xd_va.sh
# Define dependencies for each module
declare -A DEPENDENCIES_ALL
echo "Process ALL dependency"

# Function to merge dependencies
merge_dependencies() {
    local key=$1
    shift
    local -a all_deps=("$@")
    local -A unique_deps
    for dep in "${all_deps[@]}"; do
        for sub_dep in $dep; do
            unique_deps["$sub_dep"]=1
        done
    done
    DEPENDENCIES_ALL["$key"]=$(echo "${!unique_deps[@]}" | tr ' ' ' ')
}

# Collect all keys
all_keys=()
for key in "${!DEPENDENCIES_XC_VA[@]}" "${!DEPENDENCIES_XC_VB[@]}" "${!DEPENDENCIES_XD_V1[@]}" "${!DEPENDENCIES_XD_VA[@]}"; do
    all_keys+=("$key")
done
all_keys=($(echo "${all_keys[@]}" | tr ' ' '\n' | sort -u | tr '\n' ' '))

# Merge dependencies for each key
for key in "${all_keys[@]}"; do
    merge_dependencies "$key" "${DEPENDENCIES_XC_VA[$key]}" "${DEPENDENCIES_XC_VB[$key]}" "${DEPENDENCIES_XD_V1[$key]}" "${DEPENDENCIES_XD_VA[$key]}"
done

# Print merged dependencies
#for key in "${!DEPENDENCIES_ALL[@]}"; do
#    echo "[$key]=\"${DEPENDENCIES_ALL[$key]}\""
#done

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
    for dep in ${DEPENDENCIES_ALL[$module]}; do
        if [[ ! " ${PROCESSED_MODULES[@]} " =~ " ${dep} " ]]; then
            PROCESSED_MODULES+=("$dep")
            CFLAGS+=("-D$(add_prefix "$dep")")
            find_dependencies "$dep"
        fi
    done
}
echo "" > makefile.rtl907x_all_modules
# Read modules from input arguments
for module in "${!DEPENDENCIES_ALL[@]}"; do
    CFLAGS=()
    PROCESSED_MODULES=()

    echo $module
    # Add the module's own CFLAGS first
    CFLAGS+=("-D$(add_prefix "$module")")
    PROCESSED_MODULES+=("$module")
    find_dependencies "$module"
    echo "ifneq (,\$(findstring $module, \$(RTL906X_MODULES)))" >> makefile.rtl907x_all_modules
    #echo "MODULE_CFLAGS+=${CFLAGS[@]}" >> makefile.rtl907x_all_modules
    echo "CFLAGS+=${CFLAGS[@]}" >> makefile.rtl907x_all_modules
    echo "endif" >> makefile.rtl907x_all_modules
done