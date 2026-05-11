# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sanitize.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/07 02:07:43 by jaubry--          #+#    #+#              #
#    Updated: 2026/02/13 07:31:00 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ASAN_OPTIONS=intercept_tls_get_addr=0:detect_leaks=0:quarantine_size_mb=512:redzone=128:max_redzone=2048:report_globals=1:check_initialization_order=1:strict_init_order=1:detect_stack_use_after_return=0:min_uar_stack_size_log=16:max_uar_stack_size_log=20:detect_container_overflow=1:detect_odr_violation=2:strict_string_checks=1:replace_str=1:replace_intrin=1:print_stats=1:print_legend=1:atexit=1:print_full_thread_history=1:alloc_dealloc_mismatch=1:new_delete_type_mismatch=1:poison_heap=1:poison_partial=1:poison_array_cookie=1:max_malloc_fill_size=8192:allow_user_poisoning=1:check_malloc_usable_size=1:sleep_before_dying=0:verbosity=1:halt_on_error=0:dump_instruction_bytes=1:protect_shadow_gap=0:interceptor_via_fun=0

LSAN_OPTIONS="verbosity=1:log_threads=0:log_pointers=0:report_objects=0:use_registers=1:use_globals=1:use_stacks=1:use_root_regions=1:use_ld_allocations=1:use_tls=1:use_unaligned=1:print_suppressions=0:max_leaks=0:exitcode=23"
LASAN_OPTIONS="detect_leaks=1:leak_check_at_exit=1:malloc_context_size=50:print_full_thread_history=1:verbosity=1"

UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=0:verbosity=1:report_error_type=1"



# Makefile Variables
FAST		= $(if $(filter fast,$(MAKECMDGOALS)),1,0)
DEBUG		= $(if $(filter debug,$(MAKECMDGOALS)),1,0)
INSPECT		= $(if $(filter inspect,$(MAKECMDGOALS)),1,0)
PROFILE		= $(if $(filter profile,$(MAKECMDGOALS)),1,0)
SAN_MEM     = $(if $(filter san-mem,$(MAKECMDGOALS)),1,0)
SAN_LEAK    = $(if $(filter san-leak,$(MAKECMDGOALS)),1,0)
SAN_UB      = $(if $(filter san-ub,$(MAKECMDGOALS)),1,0)

ifeq ($(FAST),1)
	RULE = fast
else ifeq ($(INSPECT),1)
	RULE = inspect
else ifeq ($(PROFILE),1)
	RULE = profile
else ifeq ($(SAN_MEM),1)
	RULE = san-mem
else ifeq ($(SAN_LEAK),1)
	RULE = san-leak
else ifeq ($(SAN_UB),1)
	RULE = san-ub
else ifeq ($(DEBUG),1)
	DEBUG_LVL = 1
endif

# Flags
ifeq ($(PROFILE),1)
	PROFILE_FLAGS = -g3 -pg
else ifeq ($(INSPECT),1)
	INSPECT_FLAGS = -g3
# ============================================================================
# SANITIZER: MEMORY (AddressSanitizer) - MAXIMUM DEPTH (NO LEAK)
# ============================================================================
# Detects: heap/stack/global overflow, use-after-free, use-after-scope,
#          invalid pointer operations, initialization order issues
# Runtime: export the ASAN_OPTIONS below before running
# ============================================================================
else ifeq ($(SAN_MEM),1)
	SANITIZE_FLAGS	= address \
					  pointer-compare \
					  pointer-subtract \
					  undefined \
					  shift \
					  shift-base \
					  shift-exponent \
					  integer-divide-by-zero \
					  unreachable \
					  vla-bound \
					  null \
					  return \
					  signed-integer-overflow \
					  bounds \
#					  bounds-strict \
					  alignment \
					  object-size \
					  float-divide-by-zero \
					  float-cast-overflow \
					  nonnull-attribute \
					  returns-nonnull-attribute \
					  bool \
					  enum \
					  pointer-overflow \
					  builtin
	SANITIZE_FLAGS	:= -g3 -O1 $(addprefix -fsanitize=,$(SANITIZE_FLAGS)) \
					   -fno-omit-frame-pointer \
					   -fno-optimize-sibling-calls \
					   -fno-common \
					   -fsanitize-address-use-after-scope
# ============================================================================
# SANITIZER: LEAK - MAXIMUM DEPTH LEAK DETECTION ONLY
# ============================================================================
# Detects: all memory leaks with complete allocation traces
# Runtime: export the LSAN_OPTIONS below before running
# ============================================================================
else ifeq ($(SAN_LEAK),1)
	SANITIZE_FLAGS	= leak \
					  address
	SANITIZE_FLAGS	:= -g3 -O1 $(addprefix -fsanitize=,$(SANITIZE_FLAGS)) \
					   -fno-omit-frame-pointer \
					   -fno-common
# ============================================================================
# SANITIZER: UNDEFINED BEHAVIOR - MAXIMUM DEPTH ALL UB CHECKS
# ============================================================================
# Detects: ALL undefined behavior categories
# Runtime: export the UBSAN_OPTIONS below before running
# ============================================================================
else ifeq ($(SAN_UB),1)
	SANITIZE_FLAGS	= undefined \
					  shift \
					  shift-base \
					  shift-exponent \
					  integer-divide-by-zero \
					  unreachable \
					  vla-bound \
					  null \
					  return \
					  signed-integer-overflow \
#					  unsigned-integer-overflow \
					  bounds \
#					  bounds-strict \
					  alignment \
					  object-size \
					  float-divide-by-zero \
					  float-cast-overflow \
					  nonnull-attribute \
					  returns-nonnull-attribute \
					  bool \
					  enum \
					  pointer-overflow \
					  builtin \
					  implicit-conversion \
					  implicit-integer-truncation \
					  implicit-integer-sign-change \
					  implicit-signed-integer-truncation \
					  implicit-unsigned-integer-truncation \
					  local-bounds \
					  nullability \
					  nullability-arg \
					  nullability-assign \
					  nullability-return
	SANITIZE_FLAGS	:= -g3 -O1 $(addprefix -fsanitize=,$(SANITIZE_FLAGS)) \
					   -fno-omit-frame-pointer \
					   -fno-sanitize-recover=all
endif

ifeq ($(SAN_MEM),1)
	SAN_MODE = MEMORY SANITIZER
	SAN_EXPORT = ASAN_OPTIONS=\"$(ASAN_OPTIONS)\"
else ifeq ($(SAN_LEAK),1)
	SAN_MODE = LEAK SANITIZER
	SAN_EXPORT = LSAN_OPTIONS=\"$(LSAN_OPTIONS)\" ; export ASAN_OPTIONS=\"$(LASAN_OPTIONS)\"
else ifeq ($(SAN_UB),1)
	SAN_MODE = UNDEFINED BEHAVIOR
	SAN_EXPORT = UBSAN_OPTIONS=\"$(UBSAN_OPTIONS)\"
endif

.PHONY: debug fast inspect profile san-mem san-leak san-ub
