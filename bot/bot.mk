BOT_NAME    = ircbot
BOT_DIR     = bot/src
BOT_INCDIR  = bot/include
BOT_OBJDIR  := .obj/bot

BOT_SRCS    = $(BOT_DIR)/main.cpp       \
              $(BOT_DIR)/Bot.cpp         \
              $(BOT_DIR)/BotCommand.cpp  \
              $(BOT_DIR)/BotReply.cpp    \
              $(BOT_DIR)/BotSignals.cpp

BOT_OBJS    = $(addprefix $(BOT_OBJDIR)/, $(notdir $(BOT_SRCS:.cpp=.o)))
BOT_DEPS    = $(addprefix $(DEPDIR)/, $(notdir $(BOT_SRCS:.cpp=.d)))
BOT_HEADERS = $(shell find $(BOT_INCDIR) -type f \( -name '*.hpp' \))
