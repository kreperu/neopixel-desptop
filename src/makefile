all:
	cd build; echo -e "\033[0;32m Entering build stage"; \
	echo -e "\033[0;32m Cmake Project"; cmake ..; \
	echo -e "\033[0;32m Make Project"; make; \
	echo -e "\033[0;32m Load Project"; cp ctrl.uf2 /run/media/kuba/RPI-RP2/; \
	echo -e "\033[0;32m Done!"
mkl:
	cd build; echo -e "\033[0;32m Entering build stage"; \
	echo -e "\033[0;32m Make Project"; make; \
	echo -e "\033[0;32m Load Project"; cp ctrl.uf2 /run/media/kuba/RPI-RP2/; \
	echo -e "\033[0;32m Done!"
load:
	cd build; echo -e "\033[0;32m Entering build stage"; \
	echo -e "\033[0;32m Load Project"; cp ctrl.uf2 /run/media/kuba/RPI-RP2/; \
	echo -e "\033[0;32m Done!"
test:
	cd build; echo -e "\033[0;32m Entering build stage"; \
	echo -e "\033[0;32m Cmake Project"; cmake ..; \
	echo -e "\033[0;32m Make Project"; make; \
	echo -e "\033[0;32m Done!"
ping:
	echo -e "\033[0;32m Test!";
clean:
	rm -rf build; mkdir build; echo -e "\033[0;32m Done!"