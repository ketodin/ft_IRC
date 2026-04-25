# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/29 05:48:20 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/25 18:56:50 by jaubry--         ###   ########.fr        #
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
vpath %.hpp	$(INCLUDES)
vpath %.tpp	$(INCLUDES)
vpath %.inl	$(INCLUDES)
vpath %.h	$(INCLUDES)
vpath %.o	$(OBJDIR)
vpath %.d	$(DEPDIR)

include	$(ROOTDIR)/mkidir/make_rules.mk

$(NAME): $(OBJS) $(HEADERS)
	$(call bin-link-msg)
ifeq ($(VERBOSE),1)
	$(CF) $(OBJS) -o $@
else
	@$(CF) $(OBJS) -o $@
endif
	$(call bin-finish-msg)

check-cpp:
	@./.github/scripts/ci-run-cppcheck.sh

check-format:
	@./.github/scripts/ci-check-format.sh src include

check-all: check-cpp check-format

submodules:
	git submodule init
	git submodule sync
	git submodule update --remote

help:
	@echo "Available targets:"
	@echo -e "\tall, $(NAME)\t\t: Build the program"
	@echo -e "\tdebug\t\t\t\t: Build the program with debug symbols"
	@echo -e "\tre\t\t\t\t: Rebuild $(NAME)"
	@echo
	@echo -e "\tfast\t\t\t\t: Build with -Ofast -march=native optimizations"
	@echo -e "\trefast\t\t\t\t: Rebuild $(NAME) with fast optimizations"
	@echo
	@echo -e "\tinspect\t\t\t\t: Build with -g3 for deep inspection (no sanitizer)"
	@echo -e "\tprofile\t\t\t\t: Build with -g3 -pg for gprof profiling"
	@echo
	@echo "Sanitizers:"
	@echo -e "\tsan-mem\t\t\t\t: Build with AddressSanitizer (heap/stack overflow, use-after-free)"
	@echo -e "\tsan-leak\t\t\t: Build with LeakSanitizer (memory leak detection)"
	@echo -e "\tsan-ub\t\t\t\t: Build with UndefinedBehaviorSanitizer (all UB categories)"
	@echo -e "\tresan-mem\t\t\t: Rebuild with AddressSanitizer"
	@echo -e "\tresan-leak\t\t\t: Rebuild with LeakSanitizer"
	@echo -e "\tresan-ub\t\t\t: Rebuild with UndefinedBehaviorSanitizer"
	@echo
	@echo "Quality checks:"
	@echo -e "\tcheck-format\t\t\t: Check clang-format compliance on src/ and include/"
	@echo -e "\tcheck-cpp\t\t\t: Run cppcheck static analysis"
	@echo -e "\tcheck-all\t\t\t: Run check-format and check-cpp"
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

.PHONY: check-cpp check-format check-all submodules
