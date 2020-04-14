###############################################################################
# GDB configuration file
# Initializes debug session and defines useful commands
###############################################################################

#
# Helper functions
#

# `reload` command causes board to be reset and write ELF file to it 
define reload
	monitor reset halt
	load
	monitor reset init
end

# `rebuild` command first builds latest firmware, then loads is to the board
define rebuild
	make
	reload
end

# `reset` command brings board to initial state without firmware reload
define reset
	monitor reset init
end

#
# Session initialization
#

# Launch OpenOCD and use it as a remote target
target remote | openocd -c "gdb_port pipe; log_output openocd.log;" -f ek-tm4c123gxl.cfg
