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

/** \brief Digital inputs/outputs definitions
 **
 ** \addtogroup hal HAL
 ** \brief Hardware abstraction layer
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "gpio.h"
#include "preat.h"
#include "saio.h"

/* === Macros definitions ====================================================================== */
#define MAX_OUTPUT 6
/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

static digital_output_t outputs[MAX_OUTPUT];

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

static preat_error_t ActivateOutput(const preat_parameter_t parameters, uint8_t count) {
    uint8_t output = (uint8_t)parameters->value;

    if (output > MAX_OUTPUT) {
        return PREAT_GENERIC_ERROR;
    }

    DigitalOutputActivate(outputs[output]);
    return PREAT_NO_ERROR;
}

static preat_error_t DeactivateOutput(const preat_parameter_t parameters, uint8_t count) {
    uint8_t output = (uint8_t)parameters->value;

    if (output > MAX_OUTPUT) {
        return PREAT_GENERIC_ERROR;
    }

    DigitalOutputDeactivate(outputs[output]);
    return PREAT_NO_ERROR;
}

/* === Public function implementation ========================================================== */

bool RegisterGpioMethods(void) {
    static const struct digital_output_atributes_s LED_OUTPUT = {
        .inverted = false,
        .high_current = false,
        .open_colector = false,
    };

    bool result = true;

    outputs[0] = DigitalOutputCreate(GPIO5_0, &LED_OUTPUT);
    outputs[1] = DigitalOutputCreate(GPIO5_1, &LED_OUTPUT);
    outputs[2] = DigitalOutputCreate(GPIO5_2, &LED_OUTPUT);
    outputs[3] = DigitalOutputCreate(GPIO0_14, &LED_OUTPUT);
    outputs[4] = DigitalOutputCreate(GPIO1_11, &LED_OUTPUT);
    outputs[5] = DigitalOutputCreate(GPIO1_12, &LED_OUTPUT);

    result = result && PreatRegister(0x010, ActivateOutput, SINGLE_UINT8_PARAM);
    result = result && PreatRegister(0x010, DeactivateOutput, SINGLE_UINT8_PARAM);
    return result;
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
