# Calculate the radio core board target
string(REPLACE "/" ";" split_board_qualifiers "${BOARD_QUALIFIERS}")
list(GET split_board_qualifiers 1 target_soc)
set(board_target_radio "${BOARD}/${target_soc}/cpurad")
set(target_soc)

ExternalZephyrProject_Add(
  APPLICATION secondary
  SOURCE_DIR ${APP_DIR}/secondary
  BOARD ${board_target_radio}
  BOARD_REVISION ${BOARD_REVISION}
  )
  
 ExternalZephyrProject_Add(
  APPLICATION remote
  SOURCE_DIR ${APP_DIR}/remote
  BOARD ${board_target_radio}
  BOARD_REVISION ${BOARD_REVISION}
 )

add_dependencies(${DEFAULT_IMAGE} remote)
sysbuild_add_dependencies(FLASH ${DEFAULT_IMAGE} remote)
add_dependencies(${DEFAULT_IMAGE} secondary)
sysbuild_add_dependencies(FLASH ${DEFAULT_IMAGE} secondary)
