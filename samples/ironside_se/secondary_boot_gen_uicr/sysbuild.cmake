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
