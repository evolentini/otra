/* Copyright 2022, Laboratorio de Microprocesadores
 * Facultad de Ciencias Exactas y Tecnología
 * Universidad Nacional de Tucuman
 * http://www.microprocesadores.unt.edu.ar/
 * Copyright 2022, Esteban Volentini <evolentini@herrera.unt.edu.ar>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief Simple sample of use LPC HAL gpio functions
 **
 ** \addtogroup samples Sample projects
 ** \brief Sample projects to use as a starting point
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "crc.h"
#include "preat.h"
#include "unity.h"

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */
struct {
    bool called;
    uint8_t param_value;
    preat_error_t result;
} single_param_register;

/* === Private function declarations =========================================================== */

preat_error_t SingleParam(const preat_parameter_t parameters, uint8_t count);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

// clang-format off
static const uint8_t ACK_NO_ERROR[]          = {0x05, 0x00, 0x00, 0xa1, 0xb5};
static const uint8_t NACK_CRC_ERROR[]        = {0x07, 0x00, 0x11, 0x10, 0x01, 0xcc, 0x08};
static const uint8_t NACK_METHOD_ERROR[]     = {0x07, 0x00, 0x11, 0x10, 0x02, 0x6e, 0xe2};
static const uint8_t NACK_PARAMETERS_ERROR[] = {0x07, 0x00, 0x11, 0x10, 0x03, 0xbf, 0x97};
// clang-format on

/* === Private function implementation ========================================================= */

preat_error_t SingleParam(const preat_parameter_t parameters, uint8_t count) {
    single_param_register.called = true;
    single_param_register.param_value = (uint8_t)parameters[0].value;
    return single_param_register.result;
}

/* === Public function implementation ========================================================= */

void suiteSetUp(void) {
    PreatRegister(0x10, SingleParam, SINGLE_UINT8_PARAM);
}

void test_frame_has_crc_error(void) {
    uint8_t frame[64] = {0x07, 0x01, 0x01, 0x10, 0x01, 0xb5, 0x00};

    PreatExecute(frame);
    TEST_ASSERT_EQUAL_MEMORY(NACK_CRC_ERROR, frame, sizeof(NACK_CRC_ERROR));
}

void test_execute_undefined_function(void) {
    uint8_t frame[64] = {0x07, 0x02, 0x01, 0x10, 0x01, 0xa2, 0xcf};

    PreatExecute(frame);
    TEST_ASSERT_EQUAL_MEMORY(NACK_METHOD_ERROR, frame, sizeof(NACK_METHOD_ERROR));
}

void test_execute_funcion_with_less_parameters(void) {
    uint8_t frame[] = {0x05, 0x01, 0x00, 0xe2, 0x7f};

    PreatExecute(frame);
    TEST_ASSERT_EQUAL_MEMORY(NACK_PARAMETERS_ERROR, frame, sizeof(NACK_PARAMETERS_ERROR));
}

void test_execute_funcion_with_more_parameters(void) {
    uint8_t frame[] = {0x08, 0x01, 0x02, 0x11, 0x01, 0x02, 0x3b, 0x88};

    PreatExecute(frame);
    TEST_ASSERT_EQUAL_MEMORY(NACK_PARAMETERS_ERROR, frame, sizeof(NACK_PARAMETERS_ERROR));
}

void test_execute_single_parameter_funcion(void) {
    uint8_t frame[] = {0x07, 0x01, 0x01, 0x10, 0x01, 0xb5, 0xa3};

    single_param_register.called = false;
    single_param_register.param_value = 0xFF;
    single_param_register.result = PREAT_NO_ERROR;

    PreatExecute(frame);
    TEST_ASSERT_TRUE(single_param_register.called);
    TEST_ASSERT_EQUAL(0x01, single_param_register.param_value);
    TEST_ASSERT_EQUAL_MEMORY(ACK_NO_ERROR, frame, sizeof(ACK_NO_ERROR));
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
