*** Settings ***
Documentation    Integration test suite that runs the logger driver binary
...              (template_bin) and verifies its log output end-to-end,
...              exercising the log_enabled short-circuit gate and
...              per-subsystem severity filtering described in the README.
...              Build the binary first (see Build Instructions), then run
...              from the repo root via: robot integration/main.robot
Library          Process
Library          String

*** Variables ***
${CLI_CMD}        build/bin/template_bin
# The logger prints 4 literal spaces between the subsystem and level fields
# (see template.cpp). Built as variables here so Robot doesn't treat runs
# of spaces in an inline literal as column separators.
${NAV_INFO}       System: NAV${SPACE * 4}INFO: Position updated
${NAV_CRITICAL}   System: NAV${SPACE * 4}CRITICAL: Position exposed

*** Test Cases ***
Test Driver Runs Successfully
    [Documentation]    Sanity check: binary runs, exits cleanly, no stderr output.
    ${result}=    Run Process    ${CLI_CMD}    shell=${True}
    Should Be Equal As Integers    ${result.rc}    0
    Should Be Empty    ${result.stderr}

Test Logging Is Silent By Default
    [Documentation]    Before log_enabled is toggled on, no log lines should print --
    ...                confirms the short-circuit gate actually suppresses output,
    ...                not just that logger() would filter it.
    ${result}=    Run Process    ${CLI_CMD}    shell=${True}
    ${before}=    Fetch From Left    ${result.stdout}    Silent Mode Disabled
    Should Not Contain    ${before}    System:

Test Logging Enabled Shows Nav Messages
    [Documentation]    Once setting() and set_system_log_level() run, NAV messages
    ...                at or above the configured threshold should print.
    ${result}=    Run Process    ${CLI_CMD}    shell=${True}
    Should Contain    ${result.stdout}    ${NAV_INFO}
    Should Contain    ${result.stdout}    ${NAV_CRITICAL}

Test Per Subsystem Filtering
    [Documentation]    After raising the NAV threshold to CRITICAL, the INFO message
    ...                should stop appearing while CRITICAL keeps appearing --
    ...                confirms per-subsystem severity filtering, not just the
    ...                global on/off switch.
    ${result}=    Run Process    ${CLI_CMD}    shell=${True}
    Should Contain X Times    ${result.stdout}    ${NAV_INFO}        1
    Should Contain X Times    ${result.stdout}    ${NAV_CRITICAL}    2

Test Logging Silent Again After Disable
    [Documentation]    Calling setting() a second time should flip log_enabled back
    ...                to false -- confirms the toggle is a real state flip, not
    ...                one-directional.
    ${result}=    Run Process    ${CLI_CMD}    shell=${True}
    ${after}=    Fetch From Right    ${result.stdout}    Silent Mode Enabled
    Should Not Contain    ${after}    System:
