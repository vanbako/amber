# Timing constraints for Amber48 Gowin Arora-V top
# Define 27 MHz external system clock on input port sys_clk

# Gowin's SDC parser is line-based; avoid TCL continuations.
create_clock -name sys_clk -period 37.037 -waveform {0 18.518} [get_ports {sys_clk}]
