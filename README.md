# LWNFS

## Description

Simple filesystem based on [LWNFS article](https://lwn.net/Articles/13325/) with a massive reuse of APIs from the [libfs.c](https://elixir.bootlin.com/linux/v5.4.244/source/fs/libfs.c).

This filesystem create 3 files which shares the same counter. The counter could be read (incremented after each read) and write with usual file operations.

## Usage

```bash 
root@qemuarm64:~# insmod lwnfs.ko
root@qemuarm64:~# mkdir mnt
root@qemuarm64:~# mount -t lwnfs none mnt/
root@qemuarm64:~# ls -l mnt/
-r--r--r--    1 root     root             0 Jun  3 12:49 file1
-r--r--r--    1 root     root             0 Jun  3 12:49 file2
-r--r--r--    1 root     root             0 Jun  3 12:49 file3
root@qemuarm64:~# cat mnt/file1
0
root@qemuarm64:~# cat mnt/file1
1
root@qemuarm64:~# cat mnt/file2
2
root@qemuarm64:~# cat mnt/file3
3
root@qemuarm64:~# echo "42" > mnt/file3
root@qemuarm64:~# cat mnt/file3
42
```

## References

- https://lwn.net/Articles/13325/
- https://www.kernel.org/doc/html/next/filesystems/vfs.html
- https://elixir.bootlin.com/linux/v5.4.244/source/fs/libfs.c

