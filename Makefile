# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/29 05:48:20 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/23 03:23:35 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ROOTDIR		= .
include $(ROOTDIR)/mkidir/make_utils.mk

# Directories
SRCDIR		= src
OBJDIR		= .obj
DEPDIR		= .dep

# Includes
include	includes.mk

INCLUDES	= $(INCDIRS_IRC)

# Output
NAME		= ircserv

# Variables

VARS		= DEBUG=$(DEBUG)

# Compiler and flags
CXX			= c++

CXXFLAGS	= -Wall -Werror -Wextra \
			  -std=c++98

DFLAGS		= -MMD -MP -MF $(DEPDIR)/$*.d

IFLAGS		= $(addprefix -I,$(INCLUDES))

VFLAGS		= $(addprefix -D ,$(VARS))

CXXFLAGS	+= $(INSPECT_FLAGS) $(PROFILE_FLAGS) $(FAST_FLAGS) $(VFLAGS)

CF			= $(CXX) $(CXXFLAGS) $(IFLAGS)

# Sources
include $(SRCDIR)/srcs.mk

OBJS		= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))
DEPS		= $(addprefix $(DEPDIR)/, $(notdir $(SRCS:.cpp=.d)))

# VPATH
vpath %.hpp $(INCLUDES)
vpath %.h $(INCLUDES)
vpath %.o $(OBJDIR)
vpath %.d $(DEPDIR)

include	$(ROOTDIR)/mkidir/make_rules.mk

$(NAME): $(OBJS) $(HEADERS)
	$(call bin-link-msg)
ifeq ($(VERBOSE),1)
	$(CF) $(OBJS) -o $@
else
	@$(CF) $(OBJS) -o $@
endif
	$(call bin-finish-msg)

cppcheck:
	@./.github/scripts/run_cppcheck.sh

help:
	@echo "Available targets:"
	@echo -e "\tall, $(NAME)\t\t: Build the program"
	@echo -e "\tdebug\t\t\t\t: Build the program with debug symbols"
	@echo -e "\tre\t\t\t\t: Rebuild $(NAME)"
	@echo
	@echo -e "\tfast\t\t\t\t: Build the program with fast symbols"
	@echo
	@echo -e "\tclean\t\t\t\t: Remove object files"
	@echo -e "\tfclean\t\t\t\t: Remove object files, libraries and program"
	@echo
	@echo -e "\tprint-%\t\t\t\t: Prints makefile variable content when replacing '%'"

clean:
	$(call rm-obj-msg)
	@rm -rf $(OBJDIR) $(DEPDIR)

fclean:
	$(call rm-obj-msg)
	@rm -rf $(OBJDIR) $(DEPDIR)
	$(call rm-bin-msg)
	@rm -f $(NAME)
