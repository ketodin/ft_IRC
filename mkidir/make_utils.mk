# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    make_utils.mk                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/15 19:28:47 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/14 18:11:07 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL		:= /bin/bash

# Variables
NPROC		= $(shell nproc)
MAX_SIZE	= $(shell xrandr | grep "primary" | awk '{print $$4}' | cut -d '+' -f 1)
MAX_WIDTH	= $(shell echo $(MAX_SIZE) | cut -d 'x' -f 1)
MAX_HEIGHT	= $(shell echo $(MAX_SIZE) | cut -d 'x' -f 2)

# Dep Directories
LIBDIR		= $(ROOTDIR)/lib
MKIDIR		= $(ROOTDIR)/mkidir

include $(MKIDIR)/sanitize.mk

ifeq ($(FAST),1)
	OFLAGS		= -Ofast -march=native -mtune=native -msse3
	FFLAGS		= $(OFLAGS)
endif

# Sys binaries
STD_AR			= ar
STD_RANLIB		= ranlib

HOME_AR			= ar
HOME_RANLIB		= ranlib
HOME_GCC		= gcc-14
HOME_CFLAGS		= -Wno-error=maybe-uninitialized \
				  -Wno-error=stringop-overflow \
				  -Wno-error=alloc-size

FT_AR			= llvm-ar-12
FT_RANLIB		= llvm-ranlib-12
FT_GCC			= gcc-12

GET_ID = cat /etc/machine-id | sha256sum | cut -d' ' -f1
HOME_ID	= 6bb6eb3dbd1b58e9b11f9bba389b9fa248353ef0dd2fea7e9f1f5aeed881747d
ifeq ($(shell $(GET_ID)), $(HOME_ID))
	FAST_AR			= $(HOME_AR)
	FAST_RANLIB		= $(HOME_RANLIB)

	MLX_GCC			= $(HOME_GCC)

	CC				:= $(CC) $(HOME_CFLAGS)
	CXX				:= $(CXX) $(HOME_CFLAGS)
	
	XTEST			= 0
else
ifeq ($(PROFILE),1)
	PROFILE_FLAGS	+= -Rpass-missed=.*
endif
	FAST_AR			= $(FT_AR)
	FAST_RANLIB		= $(FT_RANLIB)

	MLX_GCC			= $(FT_GCC)
	
	XTEST			= 1
endif

# Print utils
include $(ROOTDIR)/mkidir/colors.mk
