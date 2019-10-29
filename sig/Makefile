CROSS_COMPILE=rsdk-linux-
#export CROSS_COMPILE
KSRC := /home/sarah/sdk_v2.3_release/linux-3.10
ARCH := rlx
EXTRA_CFLAGS += -Werror
MODULE_SIG_DRIVER := sig
obj-m += $(MODULE_SIG_DRIVER).o
$(MODULE_SIG_DRIVER)-objs += sig_driver.o
MAKE := PATH=/home/sarah/sdk_v2.3_release/toolchain/rsdk-4.8.5-5281-EL-3.10-u0.9.33-m32fut-161202/bin:/home/sarah/sdk_v2.3_release/config/linux/:$(PATH); make --no-print-directory

all: modules

modules:
	$(MAKE) ARCH=rlx CROSS_COMPILE=$(CROSS_COMPILE) -C /home/sarah/sdk_v2.3_release/linux-3.10 M=$(shell pwd) modules

clean:
	rm -f *.o

