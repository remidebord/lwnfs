#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/pagemap.h>
#include <linux/fs.h>
#include <linux/fs_context.h>
#include <asm/atomic.h>
#include <asm/uaccess.h>

#define LFS_MAGIC 0x19980122

static int counter;

/*
 * Files operations
 */
static ssize_t lfs_read_file(struct file *filep, char *buffer, size_t count, loff_t *offset)
{
	char tmp[32] = {0};
	int length;
	ssize_t ret;

	length = sprintf(tmp, "%d\n", counter);

	ret = simple_read_from_buffer(buffer, count, offset, tmp, length);

	if (ret > 0)
		counter++;

	return ret;
}

static ssize_t lfs_write_file(struct file *filep, const char *buffer, size_t count, loff_t *offset)
{
	counter = simple_strtol(buffer, NULL, 10);

	return count;
}

static struct file_operations lfs_file_ops = {
	.open	= simple_open,
	.read 	= lfs_read_file,
	.write  = lfs_write_file,
};

/* 
 * Superblock operations
 */
static struct super_operations lfs_s_ops = {
	.statfs		= simple_statfs,
};

/* 
 * Superblock (filesytem instance)
 */
static int lfs_fill_super(struct super_block *sb, struct fs_context *fc)
{
	int ret;

	static const struct tree_descr files[] = {
		[2] = {"file_1", &lfs_file_ops, S_IRUGO},
		[3] = {"file_2", &lfs_file_ops, S_IRUGO},
		[4] = {"file_3", &lfs_file_ops, S_IRUGO},
		{""},
	};

	sb->s_blocksize = PAGE_SIZE;
	sb->s_blocksize_bits = PAGE_SHIFT;
	sb->s_magic = LFS_MAGIC;
	sb->s_op = &lfs_s_ops;

	ret = simple_fill_super(sb, LFS_MAGIC, files);
	if (ret)
		return ret;

	return 0;
}

static int lfs_get_tree(struct fs_context *fc)
{
	return get_tree_nodev(fc, lfs_fill_super);
}

static const struct fs_context_operations lfs_context_ops = {
	.get_tree	= lfs_get_tree,
};

static int lfs_init_fs_context(struct fs_context *fc)
{
	fc->ops = &lfs_context_ops;
	return 0;
}

static struct file_system_type lfs_type = {
	.owner				= THIS_MODULE,
	.name				= "lwnfs",
	.init_fs_context	= lfs_init_fs_context,
	.kill_sb			= kill_litter_super,
};

/*
 * Register filesystem
 */
static int __init lfs_init(void)
{
	return register_filesystem(&lfs_type);
}

static void __exit lfs_exit(void)
{
	unregister_filesystem(&lfs_type);
}

module_init(lfs_init);
module_exit(lfs_exit);