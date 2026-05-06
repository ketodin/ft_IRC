# Directories
BOT_DIR      = bot

# Source files
BOT_SRCS	= main.cpp \
			  Bot.cpp \
			  BotReply.cpp \
			  BotCommand.cpp \
			  setupSignals.cpp

SRCS		+= $(addprefix $(BOT_DIR)/, $(BOT_SRCS))

# VPATH
vpath %.cpp	$(SRCDIR)
