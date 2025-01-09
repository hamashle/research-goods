#!/bin/bash
../mncore2_emuenv_20240826/assemble3 $1 >| tmp.vsm.asm
../mncore2_emuenv_20240826/gpfn3_package_main -i tmp.vsm.asm |& tee $1_result.txt