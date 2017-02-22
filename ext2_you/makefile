
o_file =  bitmap.o ext2_disk.o inode_and_path.o target_main.o sys.o

target : $(o_file)
	gcc -o target $(o_file)

sys.o : sys.c
	gcc -c sys.c

bitmap.o : bitmap.c 
	gcc -c bitmap.c

ext2_disk.o : ext2_disk.c
	gcc -c ext2_disk.c

inode_and_path.o : inode_and_path.c
	gcc -c inode_and_path.c

target_main.o : target_main.c
	gcc -c target_main.c

clean :
	rm $(o_file) target
