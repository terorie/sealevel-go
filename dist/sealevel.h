/*
 * libsealevel is a C interface for the Sealevel virtual machine.
 * This version of the library bundles the interpreter and JIT executors part of the Rust implementation of the Solana blockchain.
 *
 * Source code: https://github.com/solana-labs/solana
 *
 * ABI stability is planned, though this version makes no promises yet.
 * Avoid passing objects between two different versions of this library because no internal compatibility guarantees are made.
 *
 * Note that, despite the Rust code under the hood, this interface allows unsafe behavior.
 * The usual C rules apply library-wide: Check for null pointers, avoid aliasing, don't mix types, respect thread safety, no double frees.
 * You may find additional safety remarks on each exported function.
*/

#pragma once

/* Generated with cbindgen:0.23.0 */

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define SEALEVEL_ERR_UNKNOWN -1

#define SEALEVEL_OK 0

#define SEALEVEL_ERR_INVALID_ELF 1

#define SEALEVEL_ERR_SYSCALL_REGISTRATION 2

#define SEALEVEL_ERR_CALL_DEPTH_EXCEEDED 3

#define SEALEVEL_ERR_EXIT_ROOT_CALL_FRAME 4

#define SEALEVEL_ERR_DIVIDE_BY_ZERO 5

#define SEALEVEL_ERR_DIVIDE_OVERFLOW 6

#define SEALEVEL_ERR_EXECUTION_OVERRUN 7

#define SEALEVEL_ERR_CALL_OUTSIDE_TEXT_SEGMENT 8

#define SEALEVEL_ERR_EXCEEDED_MAX_INSTRUCTIONS 9

#define SEALEVEL_ERR_JIT_NOT_COMPILED 10

#define SEALEVEL_ERR_INVALID_VIRTUAL_ADDRESS 11

#define SEALEVEL_ERR_INVALID_MEMORY_REGION 12

#define SEALEVEL_ERR_ACCESS_VIOLATION 13

#define SEALEVEL_ERR_STACK_ACCESS_VIOLATION 14

#define SEALEVEL_ERR_INVALID_INSTRUCTION 15

#define SEALEVEL_ERR_UNSUPPORTED_INSTRUCTION 16

#define SEALEVEL_ERR_ERR_EXHAUSTED_TEXT_SEGMENT 17

#define SEALEVEL_ERR_LIBC_INVOCATION_FAILED 18

#define SEALEVEL_ERR_VERIFIER_ERROR 19

typedef enum {
  SEALEVEL_OPT_NONE,
  SEALEVEL_OPT_NO_VERIFY,
  SEALEVEL_OPT_MAX_CALL_DEPTH,
  SEALEVEL_OPT_STACK_FRAME_SIZE,
  SEALEVEL_OPT_ENABLE_STACK_FRAME_GAPS,
  SEALEVEL_OPT_INSTRUCTION_METER_CHECKPOINT_DISTANCE,
  SEALEVEL_OPT_ENABLE_INSTRUCTION_METER,
  SEALEVEL_OPT_ENABLE_INSTRUCTION_TRACING,
  SEALEVEL_OPT_ENABLE_SYMBOL_AND_SECTION_LABELS,
  SEALEVEL_OPT_DISABLE_UNRESOLVED_SYMBOLS_AT_RUNTIME,
  SEALEVEL_OPT_REJECT_BROKEN_ELFS,
  SEALEVEL_OPT_NOOP_INSTRUCTION_RATIO,
  SEALEVEL_OPT_SANITIZE_USER_PROVIDED_VALUES,
  SEALEVEL_OPT_ENCRYPT_ENVIRONMENT_REGISTERS,
  SEALEVEL_OPT_DISABLE_DEPRECATED_LOAD_INSTRUCTIONS,
  SEALEVEL_OPT_SYSCALL_BPF_FUNCTION_HASH_COLLISION,
  SEALEVEL_OPT_REJECT_CALLX_R10,
  SEALEVEL_OPT_DYNAMIC_STACK_FRAMES,
  SEALEVEL_OPT_ENABLE_SDIV,
  SEALEVEL_OPT_OPTIMIZE_RODATA,
  SEALEVEL_OPT_STATIC_SYSCALLS,
  SEALEVEL_OPT_ENABLE_ELF_VADDR,
} sealevel_config_opt;

typedef enum {
  SEALEVEL_SYSCALL_INVALID,
  SEALEVEL_SYSCALL_ABORT,
  SEALEVEL_SYSCALL_SOL_PANIC,
  SEALEVEL_SYSCALL_SOL_LOG,
  SEALEVEL_SYSCALL_SOL_LOG_64,
  SEALEVEL_SYSCALL_SOL_LOG_COMPUTE_UNITS,
  SEALEVEL_SYSCALL_SOL_LOG_PUBKEY,
  SEALEVEL_SYSCALL_SOL_CREATE_PROGRAM_ADDRESS,
  SEALEVEL_SYSCALL_SOL_TRY_FIND_PROGRAM_ADDRESS,
  SEALEVEL_SYSCALL_SOL_SHA256,
  SEALEVEL_SYSCALL_SOL_KECCAK256,
  SEALEVEL_SYSCALL_SOL_SECP256K1_RECOVER,
  SEALEVEL_SYSCALL_SOL_BLAKE3,
  SEALEVEL_SYSCALL_SOL_ZK_TOKEN_ELGAMAL_OP,
  SEALEVEL_SYSCALL_SOL_ZK_TOKEN_ELGAMAL_OP_WITH_LO_HI,
  SEALEVEL_SYSCALL_SOL_ZK_TOKEN_ELGAMAL_OP_WITH_SCALAR,
  SEALEVEL_SYSCALL_SOL_CURVE_VALIDATE_POINT,
  SEALEVEL_SYSCALL_SOL_CURVE_GROUP_OP,
  SEALEVEL_SYSCALL_SOL_GET_CLOCK_SYSVAR,
  SEALEVEL_SYSCALL_SOL_GET_EPOCH_SCHEDULE_SYSVAR,
  SEALEVEL_SYSCALL_SOL_GET_FEES_SYSVAR,
  SEALEVEL_SYSCALL_SOL_GET_RENT_SYSVAR,
  SEALEVEL_SYSCALL_SOL_MEMCPY,
  SEALEVEL_SYSCALL_SOL_MEMMOVE,
  SEALEVEL_SYSCALL_SOL_MEMCMP,
  SEALEVEL_SYSCALL_SOL_MEMSET,
  SEALEVEL_SYSCALL_SOL_INVOKE_SIGNED_C,
  SEALEVEL_SYSCALL_SOL_INVOKE_SIGNED_RUST,
  SEALEVEL_SYSCALL_SOL_ALLOC_FREE,
  SEALEVEL_SYSCALL_SOL_SET_RETURN_DATA,
  SEALEVEL_SYSCALL_SOL_GET_RETURN_DATA,
  SEALEVEL_SYSCALL_SOL_LOG_DATA,
  SEALEVEL_SYSCALL_SOL_GET_PROCESSED_SIBLING_INSTRUCTION,
  SEALEVEL_SYSCALL_SOL_GET_STACK_HEIGHT,
} sealevel_syscall_id;

/**
 * Sealevel virtual machine config.
 */
typedef struct sealevel_config sealevel_config;

/**
 * A loaded and relocated program.
 *
 * To execute this program, create a VM with `sealevel_vm_create`.
 */
typedef struct sealevel_executable sealevel_executable;

/**
 * The invoke context holds the state of a single transaction execution.
 * It tracks the execution progress (instruction being executed),
 * interfaces with account data,
 * and specifies the on-chain execution rules (precompiles, syscalls, sysvars).
 */
typedef struct sealevel_invoke_context sealevel_invoke_context;

typedef struct sealevel_vm sealevel_vm;

/**
 * The map of syscalls provided by the virtual machine.
 */
typedef void *sealevel_syscall_registry;

typedef struct {
  void *data_addr;
  size_t data_size;
  uint64_t vm_addr;
  uint64_t vm_gap_size;
  bool is_writable;
} sealevel_region;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Creates a new Sealevel machine config.
 *
 * # Safety
 * Call `sealevel_config_free` on the return value after you are done using it.
 * Failure to do so results in a memory leak.
 */
sealevel_config *sealevel_config_new(void);

/**
 * Sets a config option given the config key and exactly one value arg.
 *
 * # Safety
 * Avoid the following undefined behavior:
 * - Passing the wrong argument type as the config value (each key documents the expected value).
 */
void sealevel_config_setopt(sealevel_config *config, sealevel_config_opt key, size_t value);

/**
 * Releases resources associated with a Sealevel machine config.
 *
 * # Safety
 * Avoid the following undefined behavior:
 * - Calling this function given a string that's _not_ the return value of `sealevel_config_new`.
 * - Calling this function more than once on the same object (double free).
 * - Using the config object after calling this function (use-after-free).
 */
void sealevel_config_free(sealevel_config *config);

/**
 * Creates a new, empty syscall registry.
 *
 * # Safety
 * Call `sealevel_syscall_registry_free` on the return value after you are done using it.
 * Failure to do so results in a memory leak.
 */
sealevel_syscall_registry sealevel_syscall_registry_new(void);

/**
 * Frees a syscall registry.
 *
 * # Safety
 * Avoid the following undefined behavior:
 * - Calling this function twice on the same object.
 * - Using the syscall registry after calling this function.
 */
void sealevel_syscall_registry_free(sealevel_syscall_registry registry);

/**
 * Registers a Solana builtin syscall.
 *
 * Returns TRUE when registration was successful.
 *
 * # Safety
 * Avoid the following undefined behavior:
 * - Passing a NULL pointer.
 * - Passing a registry that was already freed with `sealevel_syscall_registry_free`.
 * - Passing a registry that was created from another thread.
 */
bool sealevel_syscall_register_builtin(sealevel_syscall_registry registry,
                                       sealevel_syscall_id syscall_id);

/**
 * Drops an invoke context and all programs created with it. Noop given a null pointer.
 *
 * # Safety
 * Avoid the following undefined behavior:
 * - Calling this function twice on the same object.
 * - Using the invoke context after calling this function.
 */
void sealevel_invoke_context_free(sealevel_invoke_context *this_);

/**
 * Returns the error code of this thread's last seen error.
 */
int sealevel_errno(void);

/**
 * Returns a UTF-8 string of this thread's last seen error.
 *
 * # Safety
 * Avoid the following undefined behavior:
 * - Writing to the error string
 * - Reading the error string after the thread that called `sealevel_strerror` exited.
 */
const char *sealevel_strerror(void);

/**
 * Loads a Sealevel program from an ELF buffer and verifies its SBF bytecode.
 *
 * Sets `sealevel_errno` and returns a null pointer if loading failed.
 *
 * Syscalls and config may be null pointers, in which case defaults are used.
 * These defaults is not stable across any libsealevel versions.
 *
 * # Safety
 * Avoid the following undefined behavior:
 * - Using the syscalls object parameter after calling this function (including a second call of this function).
 * - Providing a config object that has been freed with `sealevel_config_free` before.
 */
sealevel_executable *sealevel_load_program(const sealevel_config *config,
                                           sealevel_syscall_registry syscalls,
                                           const char *data,
                                           size_t data_len);

/**
 * Compiles a program to native executable code.
 *
 * Sets `sealevel_errno`.
 *
 * # Safety
 * Avoid the following undefined behavior:
 * - Calling this function twice on the same program.
 * - Calling this function given a null pointer or an invalid pointer.
 */
void sealevel_program_jit_compile(sealevel_executable *program);

/**
 * Creates a Sealevel virtual machine and loads the given program into it.
 *
 * Sets `sealevel_errno` and returns a null pointer if loading failed.
 *
 * The given heap should be 16-byte aligned.
 */
sealevel_vm *sealevel_vm_create(sealevel_executable *program,
                                uint8_t *heap_ptr,
                                size_t heap_len,
                                const sealevel_region *regions_ptr,
                                int regions_count);

void sealevel_vm_destroy(sealevel_vm *vm);

uint64_t sealevel_vm_execute(sealevel_vm *vm);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
