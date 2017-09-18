#!/bin/bash
VBoxManage startvm "log710h16_32_default_1452717738926_7500" --type headless
sleep 10
./sshCmd "sudo mount -t vboxsf workspace ~/workspace/"
