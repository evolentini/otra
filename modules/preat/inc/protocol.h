/************************************************************************************************
Copyright (c) 2022-2023, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnología, Universidad Nacional de Tucumán
https://www.microprocesadores.unt.edu.ar/

Copyright (c) 2022-2023, Esteban Volentini <evolentini@herrera.unt.edu.ar>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

#ifndef PROTOCOL_H
#define PROTOCOL_H

/** \brief Digital inputs/outputs declarations
 **
 ** \addtogroup hal HAL
 ** \brief Hardware abstraction layer
 ** @{ */

/* === Headers files inclusions ================================================================ */

#include <stdbool.h>
#include <stdint.h>

/* === Cabecera C++ ============================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =============================================================== */

// clang-format off
typedef enum preat_error_e {
    PREAT_NO_ERROR         = 0x00,
    PREAT_CRC_ERROR        = 0x01,
    PREAT_METHOD_ERROR     = 0x02,
    PREAT_PARAMETERS_ERROR = 0x03,
    PREAT_TOO_EARLY_ERROR  = 0x04,
    PREAT_TIMEOUT_ERROR    = 0x05,
    PREAT_UNDEFINED_ERROR  = 0x06,
    PREAT_REDEFINED_ERROR  = 0x07,
    PREAT_GENERIC_ERROR    = 0xFF,
} preat_error_t;

typedef enum preat_type_e {
    TYPE_UNDEFINED = 0x00,
    TYPE_UINT8     = 0x01,
    TYPE_UINT16    = 0x02,
    TYPE_UINT32    = 0x03,
    TYPE_BLOB      = 0x07,
    TYPE_BINARY    = 0x80,
} preat_type_t;
// clang-format on

typedef struct preat_parameter_s {
    preat_type_t type;
    uint32_t value;
} * preat_parameter_t;

typedef preat_error_t (*preat_method_t)(const preat_parameter_t parameters, uint8_t count);

/* === Public data type declarations =========================================================== */

/* === Public variable declarations ============================================================ */

extern const preat_type_t SINGLE_UINT8_PARAM[];

/* === Public function declarations ============================================================ */

bool PreatRegister(uint16_t id, preat_method_t handler, preat_type_t const * const parameters);

void PreatExecute(uint8_t * frame);

/* === End of documentation ==================================================================== */

#ifdef __cplusplus
}
#endif

/** @} End of module definition for doxygen */

#endif /* PROTOCOL_H */
