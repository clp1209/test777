#!/bin/bash
balance_core_uart(){
    first_irq=`cat /proc/interrupts  | grep rg-serial | head -n 1 | awk -F: '{print $1}'`
    last_irq=`expr $first_irq + 23`
    for i in $(seq $first_irq $last_irq)
    do
        if [ $i -ge $first_irq ] && [ $i -le `expr $first_irq + 3` ]; then
            echo "040" > /proc/irq/$i/smp_affinity
        elif [ $i -ge `expr $first_irq + 4` ] && [ $i -le `expr $first_irq + 7` ]; then
            echo "080" > /proc/irq/$i/smp_affinity
        elif [ $i -ge `expr $first_irq + 8` ] && [ $i -le `expr $first_irq + 11` ]; then
            echo "100" > /proc/irq/$i/smp_affinity
        elif [ $i -ge `expr $first_irq + 12` ] && [ $i -le `expr $first_irq + 15` ]; then
            echo "200" > /proc/irq/$i/smp_affinity
        elif [ $i -ge `expr $first_irq + 16` ] && [ $i -le `expr $first_irq + 19` ]; then
            echo "400" > /proc/irq/$i/smp_affinity
        elif [ $i -ge `expr $first_irq + 20` ] && [ $i -le $last_irq ]; then
            echo "800" > /proc/irq/$i/smp_affinity
        fi
    done
}
process=`cat /proc/cpuinfo | grep "processor" | wc -l`
if [ $process -eq 12 ]; then
    echo "====balancing uart smp affinity===="
    balance_core_uart
fi
