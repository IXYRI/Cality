# Leonardo Heap 动态化实现总结

## 核心数据结构

**Leonardo Heap 森林**：
- 由多个 Leonardo 堆组成（阶数递减，间隔 ≥ 2）
- **关键不变量**：森林中堆根**从左到右递增**
- 底层存储：连续数组
- 位向量 `p[2]`：128位标记哪些阶数的堆存在

**Leonardo 数列**：L(0)=1, L(1)=1, L(k)=L(k-1)+L(k-2)+1

---

## 操作实现

### 1. 插入 - O(log n)

```c
// 在数组末尾添加元素
head += width;
p[0] |= 1;  // 标记新的 L0 堆

// trinkle：跨森林调整，维护根递增性质
// - 沿森林向左比较
// - 必要时与左边堆的根交换
// - 复杂度：O(堆数量) = O(log n)
trinkle(head, ...);
```

**性质保证**：插入后森林根仍递增。

---

### 2. 查找 - O(log n ~ log² n)

**关键洞察**（你的发现）：
- 堆根是该堆的**上限**（最大值）
- 森林根递增 → 可以剪枝

**算法**：
```c
bool search(forest, key) {
    // 从右到左扫描森林（从大到小）
    for (heap in forest, right to left) {
        if (key == heap.root) {
            return true;
        }
        
        if (key < heap.root) {
            // key 可能在该堆内部
            // 递归搜索子堆（把子堆当作森林）
            if (heap.阶数 > 0) {
                sub_forest = [heap.left_child, heap.right_child];
                if (search(sub_forest, key)) {
                    return true;
                }
            }
        }
        // key > heap.root：该堆不可能包含 key，继续向左
    }
    return false;
}
```

**复杂度分析**：
- 最好情况：O(log n) - 在某个根中找到
- 最坏情况：O(log² n) - 需要递归进入多个堆
  - 森林有 ~log n 个堆
  - 每个堆的深度 ~log n
  - 但实际会剪枝，接近 O(log n)

---

### 3. 删除任意元素 - O(log n)

**难点**：删除中间堆会破坏森林结构。

#### 问题场景
```
原森林：L5 L3 L2
删除 L3 的根
拆分后：L5 [L2(新) L1(新)] L2(旧)
        ↑ 违反 Leonardo 规则（阶数间隔 < 2）
```

#### 解决方案（你的发现）

**核心洞察**：拆分后的序列 `L2(新) L1(新) L2(旧)` 是**有序且连续的**！

**步骤**：

1. **降根到子堆** - O(log n)
   ```c
   // 将要删除的元素上浮到堆根
   while (element 不是根) {
       swap(element, parent);  // 向上冒泡
   }
   // 因为 element ≥ 原根，不破坏子堆性质
   ```

2. **拆分堆** - O(1)
   ```c
   // L3 → L2(新) + L1(新)
   // 更新位向量标记
   ```

3. **重建局部森林** - O(1)！
   
   **关键性质**：
   - 原森林满足：L5.root < L3.root < L2.root
   - 拆分后仍满足：L5 < L2(新) < L1(新) < L2(旧)
   - 这是**有序序列**，按 smoothsort 构建逻辑自动合并：
   
   ```c
   // [L2(新) L1(新) L2(旧)] 重建
   // 因为有序，smoothsort 构建退化为线性扫描
   // 实际操作：
   // - L2 + L1 的首元素 → 合并成 L3
   // - 剩余元素形成 L1 L0
   // 结果：L5 L3 L1 L0
   ```

4. **Sift 维护堆性质** - O(log n)
   ```c
   // 如果相邻堆根违反递增
   if (L1.root > L0.root) {
       swap(L1.root, L0.root);
       sift(L0);  // 下沉维护
   }
   ```

**总复杂度**：O(log n)

**为什么重建是 O(1)**：
- 只影响 3 个相邻堆（局部操作）
- 数据已有序，无需比较或移动
- 只需更新位向量 `p`

---

### 4. 去重问题

**挑战**：Leonardo Heap 本质是大根堆，天然允许重复。

#### 方案A：插入时查找去重
```c
bool insert_unique(key) {
    if (search(forest, key)) {
        return false;  // 已存在
    }
    insert(key);
    return true;
}
// 复杂度：O(log n) 查找 + O(log n) 插入 = O(log n)
```

**代价**：每次插入都要查找。

#### 方案B：维护辅助哈希表
```c
hashset<key> keys;
leonardo_heap heap;

insert(key) {
    if (!keys.insert(key)) return false;  // O(1) 平均
    heap.insert(key);                      // O(log n)
}
```

**代价**：额外 O(n) 空间 + 失去简洁性。

#### 方案C：允许重复（Multiset 语义）
```c
// 不去重，允许重复插入
// 查找返回第一个匹配
// 删除可以删除所有匹配或单个
```

**适用场景**：优先队列、频率统计等。

---

## 性质总结

### 时间复杂度

| 操作 | 复杂度 | 备注 |
|------|--------|------|
| 插入 | O(log n) | trinkle |
| 查找 | O(log n ~ log² n) | 递归搜索，实际接近 O(log n) |
| 删除 | O(log n) | 降根 + 局部重建 |
| 去重 | O(log n) | 方案A：查找 + 插入 |

### 空间复杂度

- **O(1) 辅助空间**（不含数组本身）
- 如果需要去重：+O(n) 哈希表

### 对比红黑树

| 特性 | Leonardo Heap | 红黑树 |
|------|---------------|--------|
| **Cache locality** | ✅ 优秀（连续数组） | ❌ 差（指针跳转） |
| **查找** | O(log n ~ log² n) | ✅ O(log n) 严格 |
| **插入/删除** | O(log n) | O(log n) |
| **代码复杂度** | 中等 | 高（旋转+染色） |
| **去重** | ⚠️ 需额外机制 | ✅ 天然支持 |
| **稳定性** | 可能稳定（需验证） | 不稳定 |

---

## 适用场景

**强烈推荐**：
- ✅ **优先队列**（只删除最大/最小）
- ✅ **Multiset**（允许重复，偶尔查找）
- ✅ **批量操作** + Cache 敏感场景

**不推荐**：
- ❌ 频繁查找场景（红黑树更稳定）
- ❌ 需要严格唯一键（需额外去重开销）
- ❌ 需要范围查询/迭代器

---

## 关键创新点

1. **删除中间堆的 O(1) 局部重建**：利用有序性质
2. **无需排序的查找算法**：利用根递增 + 递归剪枝
3. **保持 Cache locality**：所有操作在连续数组上进行

这是对 smoothsort（只用于排序）的成功动态化！🎯


---
orig-smoothsort.c（stable）
```c
// 定义_BSD_SOURCE宏，启用BSD扩展功能
#define _BSD_SOURCE

// 包含标准整数类型定义（如uint8_t, int32_t等）
#include <stdint.h>

// 包含标准库函数（如malloc, free等）
#include <stdlib.h>

// 包含字符串操作函数（如memcpy, memset等）
#include <string.h>

// 包含原子操作相关的头文件
#include "atomic.h"

// 定义ntz宏为计算末尾零位数(number of trailing zeros)的函数
#define ntz(x) a_ctz_l((x))

// 定义比较函数指针类型：接受两个void*和一个void*参数，返回int
typedef int (*cmpfun)(const void *, const void *, void *);

// pntz函数：计算一个128位数(用两个size_t表示)减1后的末尾零位数
static inline int pntz(size_t p[2]) {
	// 计算低位部分p[0]-1的末尾零位数
	int r = ntz(p[0] - 1);
	
	// 如果r不为0（低位有非零位），或者高位p[1]有非零位
	if(r != 0 || (r = 8*sizeof(size_t) + ntz(p[1])) != 8*sizeof(size_t)) {
		// 返回末尾零位数
		return r;
	}
	
	// 否则返回0（整个数为0的情况）
	return 0;
}

// cycle函数：循环移位数组元素，将ar[0]移到临时缓冲区，其他元素前移
static void cycle(size_t width, unsigned char* ar[], int n)
{
	// 临时缓冲区，大小256字节
	unsigned char tmp[256];
	size_t l;  // 当前要复制的长度
	int i;     // 循环计数器

	// 如果数组元素少于2个，无需循环
	if(n < 2) {
		return;
	}

	// 将临时缓冲区设为数组的第n个元素（虚拟位置）
	ar[n] = tmp;
	
	// 当还有宽度需要处理时循环
	while(width) {
		// 取临时缓冲区大小和剩余宽度的较小值
		l = sizeof(tmp) < width ? sizeof(tmp) : width;
		
		// 将ar[0]的l字节复制到临时缓冲区
		memcpy(ar[n], ar[0], l);
		
		// 将ar[1]到ar[n]依次前移到ar[0]到ar[n-1]
		for(i = 0; i < n; i++) {
			memcpy(ar[i], ar[i + 1], l);
			ar[i] += l;  // 指针前进l字节
		}
		
		// 减少剩余宽度
		width -= l;
	}
}

/* shl()和shr()需要n > 0 */

// shl函数：128位左移（shift left）
static inline void shl(size_t p[2], int n)
{
	// 如果移位数大于等于一个size_t的位数
	if(n >= 8 * sizeof(size_t)) {
		// 调整移位数
		n -= 8 * sizeof(size_t);
		// 低位移到高位
		p[1] = p[0];
		// 低位清零
		p[0] = 0;
	}
	
	// 高位左移n位
	p[1] <<= n;
	// 高位或上低位右移后溢出的部分
	p[1] |= p[0] >> (sizeof(size_t) * 8 - n);
	// 低位左移n位
	p[0] <<= n;
}

// shr函数：128位右移（shift right）
static inline void shr(size_t p[2], int n)
{
	// 如果移位数大于等于一个size_t的位数
	if(n >= 8 * sizeof(size_t)) {
		// 调整移位数
		n -= 8 * sizeof(size_t);
		// 高位移到低位
		p[0] = p[1];
		// 高位清零
		p[1] = 0;
	}
	
	// 低位右移n位
	p[0] >>= n;
	// 低位或上高位左移后溢出的部分
	p[0] |= p[1] << (sizeof(size_t) * 8 - n);
	// 高位右移n位
	p[1] >>= n;
}

// sift函数：堆的下沉操作，维护Leonardo堆的性质
static void sift(unsigned char *head, size_t width, cmpfun cmp, void *arg, int pshift, size_t lp[])
{
	unsigned char *rt, *lf;  // 右孩子、左孩子指针
	// 数组存储需要循环移位的元素指针（最多14*sizeof(size_t)+1个Leonardo数）
	unsigned char *ar[14 * sizeof(size_t) + 1];
	int i = 1;  // 数组索引

	// 当前节点作为第一个元素
	ar[0] = head;
	
	// 当堆的阶数大于1时（即有孩子节点）
	while(pshift > 1) {
		// 右孩子位置（Leonardo堆中右孩子在左边，距离为width）
		rt = head - width;
		// 左孩子位置（距离为width + lp[pshift-2]）
		lf = head - width - lp[pshift - 2];

		// 如果当前节点大于等于左右孩子，满足堆性质
		if(cmp(ar[0], lf, arg) >= 0 && cmp(ar[0], rt, arg) >= 0) {
			break;
		}
		
		// 如果左孩子大于等于右孩子
		if(cmp(lf, rt, arg) >= 0) {
			// 记录左孩子
			ar[i++] = lf;
			// 移动到左孩子位置
			head = lf;
			// 阶数减1（左孩子的阶数）
			pshift -= 1;
		} else {
			// 记录右孩子
			ar[i++] = rt;
			// 移动到右孩子位置
			head = rt;
			// 阶数减2（右孩子的阶数）
			pshift -= 2;
		}
	}
	
	// 执行循环移位，将最大元素移到正确位置
	cycle(width, ar, i);
}

// trinkle函数：跨堆调整操作，处理多个Leonardo堆之间的关系
static void trinkle(unsigned char *head, size_t width, cmpfun cmp, void *arg, size_t pp[2], int pshift, int trusty, size_t lp[])
{
	unsigned char *stepson,  // "继子"节点（前一个堆的根）
	              *rt, *lf;  // 右孩子、左孩子
	size_t p[2];  // 位向量的副本
	// 存储需要循环移位的元素指针
	unsigned char *ar[14 * sizeof(size_t) + 1];
	int i = 1;    // 数组索引
	int trail;    // 末尾零位数

	// 复制位向量
	p[0] = pp[0];
	p[1] = pp[1];

	// 当前节点作为第一个元素
	ar[0] = head;
	
	// 当位向量不为1时（即还有其他堆）
	while(p[0] != 1 || p[1] != 0) {
		// 找到前一个堆的根（继子）
		stepson = head - lp[pshift];
		
		// 如果继子小于等于当前节点，满足性质
		if(cmp(stepson, ar[0], arg) <= 0) {
			break;
		}
		
		// 如果不可信且当前堆有孩子
		if(!trusty && pshift > 1) {
			// 计算右孩子和左孩子位置
			rt = head - width;
			lf = head - width - lp[pshift - 2];
			
			// 如果某个孩子大于等于继子，不能交换
			if(cmp(rt, stepson, arg) >= 0 || cmp(lf, stepson, arg) >= 0) {
				break;
			}
		}

		// 记录继子
		ar[i++] = stepson;
		// 移动到继子位置
		head = stepson;
		// 计算p的末尾零位数
		trail = pntz(p);
		// 右移trail位
		shr(p, trail);
		// 阶数增加trail
		pshift += trail;
		// 标记为不可信
		trusty = 0;
	}
	
	// 如果不可信（发生了交换）
	if(!trusty) {
		// 执行循环移位
		cycle(width, ar, i);
		// 对当前堆执行下沉操作
		sift(head, width, cmp, arg, pshift, lp);
	}
}

// __qsort_r：qsort_r的实际实现，使用smoothsort算法
void __qsort_r(void *base, size_t nel, size_t width, cmpfun cmp, void *arg)
{
	size_t lp[12*sizeof(size_t)];  // Leonardo数数组（按元素宽度缩放）
	size_t i, size = width * nel;  // 循环变量和总字节数
	unsigned char *head, *high;    // 当前位置和最高位置指针
	size_t p[2] = {1, 0};          // 位向量，表示Leonardo堆的结构
	int pshift = 1;                // 当前堆的阶数
	int trail;                     // 末尾零位数

	// 如果数组为空，直接返回
	if (!size) return;

	// 指向数组起始位置
	head = base;
	// 指向最后一个元素
	high = head + size - width;

	/* 预计算Leonardo数，按元素宽度缩放 */
	// Leonardo数列：L(0)=1, L(1)=1, L(n)=L(n-1)+L(n-2)+1
	// 这里加上width是因为要计算字节偏移
	for(lp[0]=lp[1]=width, i=2; (lp[i]=lp[i-2]+lp[i-1]+width) < size; i++);

	// 构建阶段：从左到右扫描数组
	while(head < high) {
		// 如果p的低两位为11（二进制）
		if((p[0] & 3) == 3) {
			// 执行下沉操作
			sift(head, width, cmp, arg, pshift, lp);
			// p右移2位
			shr(p, 2);
			// 阶数加2
			pshift += 2;
		} else {
			// 如果下一个Leonardo堆会超出数组范围
			if(lp[pshift - 1] >= high - head) {
				// 执行trinkle操作（跨堆调整）
				trinkle(head, width, cmp, arg, p, pshift, 0, lp);
			} else {
				// 否则执行普通的下沉操作
				sift(head, width, cmp, arg, pshift, lp);
			}

			// 更新位向量和阶数
			if(pshift == 1) {
				// 如果阶数为1，左移1位，阶数变为0
				shl(p, 1);
				pshift = 0;
			} else {
				// 否则左移pshift-1位，阶数变为1
				shl(p, pshift - 1);
				pshift = 1;
			}
		}

		// 位向量最低位置1（添加新堆）
		p[0] |= 1;
		// 移动到下一个元素
		head += width;
	}

	// 对最后一个元素执行trinkle
	trinkle(head, width, cmp, arg, p, pshift, 0, lp);

	// 拆解阶段：从右到左拆解Leonardo堆
	while(pshift != 1 || p[0] != 1 || p[1] != 0) {
		// 如果阶数小于等于1
		if(pshift <= 1) {
			// 计算p的末尾零位数
			trail = pntz(p);
			// 右移trail位
			shr(p, trail);
			// 阶数增加trail
			pshift += trail;
		} else {
			// 否则阶数大于1，将当前堆拆分为两个子堆
			// 左移2位
			shl(p, 2);
			// 阶数减2
			pshift -= 2;
			// 异或7（二进制111），相当于设置三个堆的标记
			p[0] ^= 7;
			// 右移1位
			shr(p, 1);
			// 对左孩子执行trinkle（trusty=1表示可信）
			trinkle(head - lp[pshift] - width, width, cmp, arg, p, pshift + 1, 1, lp);
			// 左移1位
			shl(p, 1);
			// 最低位置1
			p[0] |= 1;
			// 对右孩子执行trinkle
			trinkle(head - width, width, cmp, arg, p, pshift, 1, lp);
		}
		// 向前移动一个元素
		head -= width;
	}
}

// weak_alias：创建弱符号别名，qsort_r指向__qsort_r
weak_alias(__qsort_r, qsort_r);
```

---

# 增量方法
- 呈leonardo數列增長（lp多算一次）
- 遷移法：數據量多於一次遷移量時使用增量遷移，一次操作遷移一個遷移量，並更新遊標。

## 結構
\+ migno（內嵌）

### migno
- 新數組地址
- valid 遊標

- 寫兩套函數，操作前判斷migno非空選擇另一套根據遊標選擇訪問數組的函數

## 行爲透明
- 用entry和特殊的只對k的比較函數實現表
- maysepkv hashk noalloc 標識位
- noalloc 允許報錯後用戶傳入新數組地址進行遷移

## 邊界情況
未遷移完成又加入大量數據超出新數組：
- 如果遷移未過半：新分配一個數組，將已遷移數據複製到新數組。舊數組遷移進度不變。
- 如果過半：一次性完成舊遷移。

開銷：因爲這種情況幾乎不可能僅由單個數據插入操作造成，考慮到一次性大量插入本身的時間複雜度，我認爲可以接受。