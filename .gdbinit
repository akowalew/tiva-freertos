###############################################################################
# GDB configuration file
# Initializes debug session and defines useful commands
###############################################################################

#
# Helper functions
#

# `reload` command causes board to be reset and write ELF file to it 
define reload
	make
	monitor reset halt
	load
	monitor reset init
end

# `reset` command brings board to initial state without firmware reload
define reset
	monitor reset init
end

define registers
	info registers
end

define faultstat
	x 0xE000ED28
end

define faultaddr
	x 0xE000ED38
end

#
# Session initialization
#

# Launch OpenOCD and use it as a remote target
target remote | openocd -c "gdb_port pipe; log_output openocd.log;" -f ek-tm4c123gxl.cfg
