# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    colors.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/07 15:57:36 by jaubry--          #+#    #+#              #
#    Updated: 2026/02/12 07:53:57 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Formatting
RESET		= \\033[0m
BOLD		= \\033[1m
UNDERLINE	= \\033[4m
NOUNDERLINE	= \\033[24m
NL			= \\n

# Color palette
RED			= \\033[38;5;196m
YELLOW		= \\033[38;5;220m
GREEN		= \\033[38;5;40m
CYAN		= \\033[38;5;51m
BLUE		= \\033[38;5;33m
ORANGE		= \\033[38;5;214m
PURPLE		= \\033[38;5;129m

# Print utils
SILENCE		= 1> /dev/null
MUTE		= $(SILENCE) 2> /dev/null

ifeq ($(DEBUG),1)
	MODE = [DEBUG]
else ifeq ($(INSPECT),1)
	MODE = [INSPECT]
else ifeq ($(PROFILE),1)
	MODE = [PROFILE]
else ifeq ($(SAN_MEM),1)
	MODE = [MEMORY-SANITIZER]
else ifeq ($(SAN_LEAK),1)
	MODE = [LEAK-SANITIZER]
else ifeq ($(SAN_UB),1)
	MODE = [UNDEFINED-BEHAVIOUR-SANITIZER]
endif

DEBUG_MODE = 0
ifeq ($(filter $(DEBUG) $(INSPECT) $(PROFILE) $(SAN_MEM) $(SAN_LEAK) $(SAN_UB), 1),1)
	DEBUG_MODE = 1
endif

define color
	@echo -e "$(1)$(subst %UL%,$(UNDERLINE),$(subst %NUL%,$(NOUNDERLINE),$(2)))$(RESET)"
endef

# LIB
define ar-msg
$(if $(filter $(DEBUG_MODE),1),\
	$(call color,$(YELLOW)$(BOLD),"$(NL)⚠ Creating $(MODE) library archive %UL%$@%NUL%..."),\
	$(call color,$(PURPLE)$(BOLD),"$(NL)〙Creating library archive %UL%$@%NUL%...")
)
endef

define lib-build-msg
$(if $(filter $(DEBUG_MODE),1),\
	$(call color,$(YELLOW)$(BOLD),"$(NL)⚠ Compiling %UL%$(NAME)%NUL% library objects in $(MODE) mode..."),\
	$(call color,$(PURPLE)$(BOLD),"$(NL)〙Compiling %UL%$(NAME)%NUL% library objects...")
)
endef

define lib-compile-obj-msg
	$(call color,$(BLUE),"╰┈➤ Compiling %UL%$(CDIR)/$<")
endef

define ar-finish-msg
$(if $(filter $(DEBUG_MODE),1),\
	$(call color,$(ORANGE)$(BOLD),"✓ Library archive %UL%$@%NUL% $(MODE) build complete"),\
	$(call color,$(GREEN)$(BOLD),"✓ Library archive %UL%$@%NUL% successfully created!")
)
endef

define rm-lib-msg
	$(call color,$(RED),"Removing library %UL%$(NAME)")
endef


# BIN
define bin-link-msg
$(if $(filter $(DEBUG_MODE),1),\
	$(call color,$(YELLOW)$(BOLD),"$(NL)⚠ Linking %UL%$@%NUL% with $(MODE) mode..."),\
	$(call color,$(PURPLE)$(BOLD),"$(NL)〙Linking program %UL%$@%NUL%...")
)
endef

define bin-build-msg
$(if $(filter $(DEBUG_MODE),1),\
	$(call color,$(YELLOW)$(BOLD),"$(NL)⚠ Compiling %UL%$(NAME)%NUL% binary objects in $(MODE) mode..."),\
	$(call color,$(PURPLE)$(BOLD),"$(NL)〙Compiling %UL%$(NAME)%NUL% binary objects...")
)
endef

define bin-compile-obj-msg
	$(call color,$(BLUE),"╰┈➤ Compiling %UL%$<")
endef

MINIRT_CMD = junest -n RUSTICL_ENABLE='radeonsi' ./$(NAME) minirt-assets/42.rt
define san-msg
	$(call color,$(YELLOW)$(BOLD),$(NL)=== $(SAN_MODE) BUILD COMPLETE ===$(NL)Run with:$(NL)$(NL)export $(SAN_EXPORT) ; $(MINIRT_CMD)$(NL))
endef

define bin-finish-msg
$(if $(filter $(DEBUG_MODE),1),\
	$(call color,$(ORANGE)$(BOLD),"✓ Binary %UL%$@%NUL% $(MODE) build complete"),\
	$(call color,$(GREEN)$(BOLD),"✓ Program %UL%$@%NUL% successfully created!")
)
$(if $(filter $(SAN_MEM) $(SAN_LEAK) $(SAN_UB),1),$(call san-msg),)
endef

define rm-bin-msg
	$(call color,$(RED),"Removing binary %UL%$(NAME)")
endef


# MLX
define mlx-build-msg
	$(call color,$(PURPLE)$(BOLD),"$(NL)〙Building the minilibx library...")
endef

define mlx-finish-msg
	$(call color,$(GREEN)$(BOLD),"✓ Minilibx build successfully completed")
endef

define mlx-clean-msg
	$(call color,$(RED),"Cleaning minilibx folder")
endef

# COMMON

define create-dir-msg
	$(call color,$(CYAN),"Creating directory %UL%$(CDIR)/$@")
endef

define rm-obj-msg
	$(call color,$(RED),"Cleaning %UL%$(NAME)%NUL% object files from %UL%$(OBJDIR)%NUL% and %UL%$(DEPDIR)")
endef

