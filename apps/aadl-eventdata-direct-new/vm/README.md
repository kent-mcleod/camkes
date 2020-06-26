# aadl-eventdata-direct-new (VM components)

This modified camkes app (aadl-eventdata-direct-new) demonstrates AADL eventdata
connectors between native camkes components and guest VMs. Starting with the baseline
scenario of a native sender and native receiver, each of the sender or receiver could
be replaced with a VM component. The combination of substitutions gives 4 scenarios.
See at the bottom for checking out the sources.

There are 2 VM guest applications, `receiver` and `sender`. They can be started from
the shell and require an argument to the device file corresponding to the VM connector,
this is `/dev/uio0`, followed by an argument for the size of the dataport, `4096`.
The implementations of these apps then use the queue interface and file descriptor
to interact with the shared memory and signalling mechanisms in a similar way to the
native applications.  In the following sections, there is a snippet of the camkes
composition describing the scenario, followed by expected console output from
running the applications on an odroid-xu4. The VM scenarios require interaction on
the console to start the Linux applications once the VMs have booted.

Switching between the scenarios should be done by checking out the commit for each scenario as
there are domain configuration changes as well as camkes `composition` changes for
some scenarios:

```
commit f17165687d3b1982649ae27e78dacedbe87ffff4
Author: Kent McLeod <Kent.Mcleod@data61.csiro.au>
Date:   Fri Jun 26 18:32:50 2020 +1000

    Scenario 2
    
    Native sender, VM receiver, Native receiver.

commit 991c131810a72af40f491440dd3f871435bd976c
Author: Kent McLeod <Kent.Mcleod@data61.csiro.au>
Date:   Fri Jun 26 18:32:17 2020 +1000

    Scenario 1
    
    VM sender, VM receiver, Native receiver

```



When using 2 VMs, the serial server is used to switch input between each VM. The serial server has an escape
character `@` for switching inputs. Switching to client 1 from client 0 would be by `@1`.
```
 --- SerialServer help ---
 Escape char: @
 0 - 1  switches input to that client
 ?      shows this help
 m      simultaneous multi-client input
 d      switch between debugging modes
          0: no debugging
          1: debug multi-input mode output coalescing
          2: debug flush_buffer_line
```

To start the receiver:
```
Welcome to Buildroot
buildroot login: root
# receiver /dev/uio0 4096
```

To start the sender:
```
Welcome to Buildroot
buildroot login: root
# sender /dev/uio0 4096
sender: sending: 1
sender: sending: 2
sender: sending: 3
Receiver: received: 1  numDropped: 0
Receiver: received: 2  numDropped: 0
Receiver: received: 3  numDropped: 0
sender: sending: 4
sender: sending: 5
sender: sending: 6
sender: sending: 7
sender: sending: 8
sender: sending: 9
Receiver: received: 4  numDropped: 0
```

## Checkout the sources

### From a different project manifest
```
repo init -u https://github.com/SEL4PROJ/camkes-arm-vm-manifest.git --depth=1
repo sync -j8
(cd projects && git clone https://github.com/kent-mcleod/camkes.git -b kent/aadl)
ln -sf projects/camkes/easy-settings.cmake
```

### From the manifest in this project

```
repo init -u https://github.com/kent-mcleod/camkes.git -b kent/aadl --depth=1
repo sync -j8
```

## Configuring for odroid-xu4

```
mkdir build
cd build
../init-build.sh -DPLATFORM=exynos5422 -DCAMKES_APP=aadl-eventdata-direct-new -DARM_HYP=ON -DARM=ON
ninja
```

## Configuring for qemu-arm-virt

You need to have a version of qemu-system-aarch64 version 4.0.0 or greater.

It is posible to specify a version of qemu via `-DQEMU_BINARY`. Otherwise whatever qemu-system-aarch64 is in the path will be used.

```
mkdir build
cd build
../init-build.sh -DPLATFORM=qemu-arm-virt -DARM_HYP=ON -DQEMU_BINARY=/tmp/tmp.8nVqEXa2QZ/qemu/aarch64-softmmu/qemu-system-aarch64 -DCAMKES_APP=aadl-eventdata-direct-new
ninja
```

Running:
```
qemu-system-aarch64 -machine virt,virtualization=on,highmem=off,secure=off -cpu cortex-a53 -nographic  -m size=1024  -kernel images/capdl-loader-image-arm-qemu-arm-virt
```

