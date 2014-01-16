AS=as
LD=ld 
.s.o:
	${AS} -a $< -o $*.o >$*.map
final: bootsect
	dd if=bootsect of=diska.img count=1
bootsect: bootsect.o
	${LD} --oformat binary -N -e start -Ttext 0x0 -o  bootsect $<
