Overview
========

The **STM32G4** library provides a hardware abstraction layer and peripheral
drivers for the STM32G491 microcontroller.

Project Layout
--------------

.. code-block:: text

   src/
   ├── lib/
   │   ├── chip/
   │   │   └── stm32g491/      # Chip-level HAL, mock, and driver
   │   └── peripherals/
   │       └── gpio/           # GPIO peripheral driver
   ├── main.cc
   └── types.hh                # Shared type definitions

Building
--------

Run Doxygen first, then Sphinx:

.. code-block:: bash

   # From project root
   doxygen Doxyfile
   sphinx-build -b html docs docs/_build/html

