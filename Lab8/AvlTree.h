#pragma once
#include <iostream>
#include <stdio.h>
using namespace std;

struct AVLnode;
typedef AVLnode* AVLtree;
struct AVLnode {
	int data, high;//Ȩֵ������
	int freq, size;//Ƶ������С
	AVLtree ls, rs;//���ӣ�����
	AVLnode() : data(0), high(1), freq(1), size(1), ls(NULL), rs(NULL) {}
	AVLnode(int a) : data(a), high(1), freq(1), size(1), ls(NULL), rs(NULL) {}//��ʼ��
}; 
AVLtree root;
inline int GetSize(AVLtree p) {//��ȡ��С
	if (p == NULL) return 0;
	return p->size;
}
inline int GetHigh(AVLtree p) {//��ȡ����
	if (p == NULL) return 0;
	return p->high;
}
inline void update(AVLtree& p) {//���½ڵ�
	p->size = GetSize(p->ls) + GetSize(p->rs) + p->freq;
	p->high = max(GetHigh(p->ls), GetHigh(p->rs)) + 1;
}
inline void LeftPlus(AVLtree& p) {
	AVLtree q;
	q = p->ls;
	p->ls = q->rs;
	q->rs = p;
	update(p);
	update(q);
	p = q;
}
inline void RightPlus(AVLtree& p) {
	AVLtree q;
	q = p->rs;
	p->rs = q->ls;
	q->ls = p;
	update(p);
	update(q);
	p = q;
}
inline void LeftRight(AVLtree& p) {//����
	RightPlus(p->ls);
	LeftPlus(p);
}
inline void RightLeft(AVLtree& p) {//����
	LeftPlus(p->rs);
	RightPlus(p);
}
inline void Insert(AVLtree& p, int x) {
	if (p == NULL) {
		p = new AVLnode(x);//û������ڵ㣬ֱ�Ӳ���һ��
		return;
	}
	if (p->data == x) {//����Ѿ���������ˣ�ֱ�������������Ƶ�ʣ���������ڵ㼴��
		++(p->freq);
		update(p);
		return;
	}
	if (p->data > x) {//�����������룬����������ƫ��
		Insert(p->ls, x), update(p);
		if (GetHigh(p->ls) - GetHigh(p->rs) == 2) {
			if (x < p->ls->data)
				LeftPlus(p);//����
			else
				LeftRight(p);//����
		}
	}
	else {//�����������룬����������ƫ��
		Insert(p->rs, x), update(p);
		if (GetHigh(p->rs) - GetHigh(p->ls) == 2) {
			if (x > p->rs->data)
				RightPlus(p);//����
			else
				RightLeft(p);//����
		}
	}
	update(p);//�����Ǹ���
}
inline void insert(int x) {
	Insert(root, x);
}
inline void Erase(AVLtree& p, int x) {
	if (p == NULL) return;//�Ҳ����������ֱ�ӷ���
	if (p->data > x) {//ɾ����������������������ƫ��
		Erase(p->ls, x), update(p);
		if (GetHigh(p->rs) - GetHigh(p->ls) == 2) {
			if (GetHigh(p->rs->rs) >= GetHigh(p->rs->ls))//һ��Ҫ�ӵȺţ�ͬ�£�������Ϊ��������X�m92�ֵ���55���ӣ�
				RightPlus(p);
			else
				RightLeft(p);
		}
	}
	else if (p->data < x) {
		Erase(p->rs, x), update(p);
		if (GetHigh(p->ls) - GetHigh(p->rs) == 2) {
			if (GetHigh(p->ls->ls) >= GetHigh(p->ls->rs))
				LeftPlus(p);
			else
				LeftRight(p);
		}
	}
	else {
		if (p->freq > 1) {//����������Ƶ�ʴ���1����ôֱ�Ӽ�ȥһ���Ϳ�����
			--(p->freq);
			update(p);
			return;
		}
		if (p->ls && p->rs) {//������������
			AVLtree q = p->rs;//��������ĺ��
			while (q->ls) q = q->ls;
			p->freq = q->freq;
			p->data = q->data, q->freq = 1;//��q�ڵ�������
			Erase(p->rs, q->data);//����ڵ�϶�����2�������ˣ�ֱ��ɾ��
			update(p);//�����Ǹ���
			if (GetHigh(p->ls) - GetHigh(p->rs) == 2) {
				if (GetHigh(p->ls->ls) >= GetHigh(p->ls->rs))
					LeftPlus(p);
				else
					LeftRight(p);
			}
		}
		else {//���ֻ��һ��������ֱ�Ӱ�����ڵ���������������ɣ�����Ҫ����
			AVLtree q = p;
			if (p->ls) p = p->ls;
			else if (p->rs) p = p->rs;
			else p = NULL;
			delete q;
			q = NULL;
		}
	}
	if (p == NULL) return;//ע�����ﻹҪ�жϣ�������ܻ�RE
	update(p);//������һ��
}
inline void erase(int x) {
	Erase(root, x);
}
inline int get_rank(AVLtree p, int val) {
	if (p->data == val) return GetSize(p->ls) + 1;//�������ڵ����Ҫ�ҵ����֣������������Ĵ�С��1
	if (p->data > val) return get_rank(p->ls, val);//�������ڵ����Ҫ�ҵ����֣�������
	return get_rank(p->rs, val) + GetSize(p->ls) + p->freq;//�����ң�����ֵҪ�����������Ĵ�С������ڵ������ֵ�Ƶ��
}
inline int GetRank(int val) {
	return get_rank(root, val);
}
inline int get_val(AVLtree p, int rank) {
	if (GetSize(p->ls) >= rank) return get_val(p->ls, rank);//����������Ĵ�С��С��������������
	if (GetSize(p->ls) + p->freq >= rank) return p->data;//����������Ĵ�С��������ڵ���ֵ���ֵ�Ƶ����С�����������������ֵ
	return get_val(p->rs, rank - GetSize(p->ls) - p->freq);//�����ң���Ҫ����Ҫ��ȥ�������Ĵ�С������ڵ���ֵ���ֵ�Ƶ��
}
inline int GetVal(int rank) {
	return get_val(root, rank);
}
inline int GetPrev(int val) {//��ǰ��
	AVLtree ans = new AVLnode(-1LL << 42), p = root;//�Ӹ��ڵ㿪ʼ�ң���ʼ�𰸸���Сֵ
	while (p) {//���p�ڵ㲻Ϊ�գ���һֱ��
		if (p->data == val) {
			if (p->ls) {//����ҵ�������ˣ����������������������һֱ������
				p = p->ls;
				while (p->rs)
					p = p->rs;
				ans = p;
			}
			break;
		}
		if (p->data < val && p->data > ans->data) ans = p;//�������һ�������ֵС�����ڵ�ǰ�𰸵�ֵ�Ļ����Ѵ𰸸���ans
		p = p->data < val ? p->rs : p->ls;
	}
	return ans->data;
}
inline int GetNext(int val) {//�Һ�̣�����ǰ������
	AVLtree ans = new AVLnode(1LL << 42), p = root;
	while (p) {
		if (p->data == val) {
			if (p->rs) {
				p = p->rs;
				while (p->ls)
					p = p->ls;
				ans = p;
			}
			break;
		}
		if (p->data > val && p->data < ans->data) ans = p;
		p = p->data < val ? p->rs : p->ls;
	}
	return ans->data;
}

/*
��˺
struct AVLnode;
typedef AVLnode* AVLtree;
struct AVLnode {
	int data, high;
	int freq, size;
	AVLtree ls, rs;
	AVLnode() : data(0), high(1), freq(1), size(1), ls(NULL), rs(NULL) {}
	AVLnode(int a) : data(a), high(1), freq(1), size(1), ls(NULL), rs(NULL) {}
};
inline int GetSize(AVLtree p) {
	if (p == NULL) return 0;
	return p->size;
}
inline int GetHigh(AVLtree p) {
	if (p == NULL) return 0;
	return p->high;
}
struct AVL {
	AVLtree root;
	inline void update(AVLtree& p) {
		p->size = GetSize(p->ls) + GetSize(p->rs) + p->freq;
		p->high = max(GetHigh(p->ls), GetHigh(p->rs)) + 1;
	}
	inline void LeftPlus(AVLtree& p) {
		AVLtree q;
		q = p->ls;
		p->ls = q->rs;
		q->rs = p;
		update(p);
		update(q);
		p = q;
	}
	inline void RightPlus(AVLtree& p) {
		AVLtree q;
		q = p->rs;
		p->rs = q->ls;
		q->ls = p;
		update(p);
		update(q);
		p = q;
	}
	inline void LeftRight(AVLtree& p) {
		RightPlus(p->ls);
		LeftPlus(p);
	}
	inline void RightLeft(AVLtree& p) {
		LeftPlus(p->rs);
		RightPlus(p);
	}
	inline void OutPut(AVLtree p) {
		if (p == NULL) return;
		OutPut(p->ls);
		for (int i = 1; i <= p->freq; ++i)
			write(p->data), putchar(32);
		OutPut(p->rs);
	}
	inline void output() {
		OutPut(root);
	}
inline void Insert(AVLtree& p, int x) {
	if (p == NULL) {
		p = new AVLnode(x);
		return;
	}
	if (p->data == x) {
		++(p->freq);
		update(p);
		return;
	}
	if (p->data > x) {
		Insert(p->ls, x), update(p);
		if (GetHigh(p->ls) - GetHigh(p->rs) == 2) {
			if (x < p->ls->data)
				LeftPlus(p);
			else
				LeftRight(p);
		}
	}
	else {
		Insert(p->rs, x), update(p);
		if (GetHigh(p->rs) - GetHigh(p->ls) == 2) {
			if (x > p->rs->data)
				RightPlus(p);
			else
				RightLeft(p);
		}
	}
	update(p);
}
inline void insert(int x) {
	Insert(root, x);
}
inline void Erase(AVLtree& p, int x) {
	if (p == NULL) return;
	if (p->data > x) {
		Erase(p->ls, x), update(p);
		if (GetHigh(p->rs) - GetHigh(p->ls) == 2) {
			if (GetHigh(p->rs->rs) >= GetHigh(p->rs->ls))
				RightPlus(p);
			else
				RightLeft(p);
		}
	}
	else if (p->data < x) {
		Erase(p->rs, x), update(p);
		if (GetHigh(p->ls) - GetHigh(p->rs) == 2) {
			if (GetHigh(p->ls->ls) >= GetHigh(p->ls->rs))
				LeftPlus(p);
			else
				LeftRight(p);
		}
	}
	else {
		if (p->freq > 1) {
			--(p->freq);
			update(p);
			return;
		}
		if (p->ls && p->rs) {
			AVLtree q = p->rs;
			while (q->ls) q = q->ls;
			p->freq = q->freq;
			p->data = q->data, q->freq = 1;
			Erase(p->rs, q->data);
			update(p);
			if (GetHigh(p->ls) - GetHigh(p->rs) == 2) {
				if (GetHigh(p->ls->ls) >= GetHigh(p->ls->rs))
					LeftPlus(p);
				else
					LeftRight(p);
			}
		}
		else {
			AVLtree q = p;
			if (p->ls) p = p->ls;
			else if (p->rs) p = p->rs;
			else p = NULL;
			delete q;
			q = NULL;
		}
	}
	if (p == NULL) return;
	update(p);
}
inline void erase(int x) {
	Erase(root, x);
}
inline int get_val(AVLtree p, int rank) {
	if (GetSize(p->ls) >= rank) return get_val(p->ls, rank);
	if (GetSize(p->ls) + p->freq >= rank) return p->data;
	return get_val(p->rs, rank - GetSize(p->ls) - p->freq);
}
inline int GetVal(int rank) {
	return get_val(root, rank);
}
inline int get_rank(AVLtree p, int val) {
	if (p->data == val) return GetSize(p->ls) + 1;
	if (p->data > val) return get_rank(p->ls, val);
	return get_rank(p->rs, val) + GetSize(p->ls) + p->freq;
}
inline int GetRank(int val) {
	return get_rank(root, val);
}
inline int GetPrev(int val) {
	AVLtree ans = new AVLnode(-1LL << 42), p = root;
	while (p) {
		if (p->data == val) {
			if (p->ls) {
				p = p->ls;
				while (p->rs)
					p = p->rs;
				ans = p;
			}
			break;
		}
		if (p->data < val && p->data > ans->data) ans = p;
		p = p->data < val ? p->rs : p->ls;
	}
	return ans->data;
}
inline int GetNext(int val) {
	AVLtree ans = new AVLnode(1LL << 42), p = root;
	while (p) {
		if (p->data == val) {
			if (p->rs) {
				p = p->rs;
				while (p->ls)
					p = p->ls;
				ans = p;
			}
			break;
		}
		if (p->data > val && p->data < ans->data) ans = p;
		p = p->data < val ? p->rs : p->ls;
	}
	return ans->data;
}
};
*/

/*
struct avlnode;
typedef avlnode* avltree;
struct avlnode
{
	int size, freq;
	int data, high;
	avltree ls, rs;
	avlnode() : data(0), high(0), size(1), freq(1), ls(NULL), rs(NULL) {}
	avlnode(int a) :data(a), high(0), size(1), freq(1), ls(NULL), rs(NULL) {}
};
inline int getsize(avltree p)
{
	if (p == NULL)
		return 0;
	else
		return p->size;
}
inline int gethigh(avltree p)
{
	if (p == NULL)
		return 0;
	else
		return p->high;
}
struct avl
{
	avltree root;
	inline void update(avltree& p)
	{
		p->size = getsize(p->ls) + getsize(p->rs) + p->freq;
		p->high = max(gethigh(p->ls), gethigh(p->rs)) + 1;
	}
	inline void leftplus(avltree& p)
	{
		avltree q = p->ls;
		p->ls = q->rs;
		q->rs = p;
		update(p);
		update(q);
		p = q;
	}
	inline void rightplus(avltree& p)
	{
		avltree q = p->rs;
		p->rs = q->ls;
		q->ls = p;
		update(p);
		update(q);
		p = q;
	}
	inline void leftright(avltree& p)
	{
		rightplus(p->ls);
		leftplus(p);
	}
	inline void rightleft(avltree& p)
	{
		leftplus(p->rs);
		rightplus(p);
	}
	inline void Insert(avltree& p, int x)
	{
		if (p == NULL)
		{
			p = new avlnode(x);
			return;
		}
		if (p->data == x)
		{
			p->freq = p->freq + 1;
			update(p);
			return;
		}
		if (p->data > x)
		{
			Insert(p->ls, x);
			update(p);
			if (gethigh(p->ls) - gethigh(p->rs) == 2)
			{
				if (x < p->ls->data)
				{
					leftplus(p);
				}
				else
				{
					leftright(p);
				}
			}
		}
		else
		{
			Insert(p->rs, x);
			update(p);
			if (gethigh(p->rs) - gethigh(p->ls) == 2)
			{
				if (x < p->rs->data)
				{
					rightleft(p);
				}
				else
				{
					rightplus(p);
				}
			}
		}
		update(p);
	}
	inline void insert(int x)
	{
		Insert(root, x);
	}
	inline void Erase(avltree& p, int x)
	{
		if (p == NULL)
		{
			return;
		}
		if (p->data > x)
		{
			Erase(p->ls, x);
			update(p);
			if (gethigh(p->rs) - gethigh(p->ls) == 2)
			{
				if (gethigh(p->rs->rs) >= gethigh(p->rs->ls))
				{
					rightplus(p);
				}
				else
				{
					rightleft(p);
				}
			}
		}
		else if (p->data < x)
		{
			Erase(p->rs, x);
			update(p);
			if (gethigh(p->ls) - gethigh(p->rs) == 2)
			{
				if (gethigh(p->ls->ls) >= gethigh(p->ls->rs))
				{
					leftplus(p);
				}
				else
				{
					leftright(p);
				}
			}
		}
		else
		{
			if (p->freq > 1)
			{
				p->freq = p->freq - 1;
				update(p);
				return;
			}
			if (p->ls && p->rs)
			{
				avltree q = p->rs;
				while (q->ls)
				{
					q = q->ls;
				}
				p->freq = q->freq, p->data = q->data, q->freq = 1;
				Erase(p->rs, p->data);
				update(p);
				if (gethigh(p->ls) - gethigh(p->rs) == 2)
				{
					if (gethigh(p->ls->ls) >= gethigh(p->ls->rs))
					{
						leftplus(p);
					}
					else
					{
						leftright(p);
					}
				}
			}
			else
			{
				avltree q = p;
				if (p->ls)
				{
					p = p->ls;
				}
				else if (p->rs)
				{
					p = p->rs;
				}
				else
				{
					p = NULL;
				}
				delete q;
				q = NULL;
			}
		}
		if (p == NULL)
		{
			return;
		}
		update(p);
	}
	inline void erase(int x)
	{
		Erase(root, x);
	}
	inline int get_rank(avltree& p, int val)
	{
		if (p->data == val)
			return getsize(p->ls) + 1;
		if (p->data > val)
			return get_rank(p->ls, val);
		return get_rank(p->rs, val) + getsize(p->ls) + p->freq;
	}
	inline int getrank(int val)
	{
		return get_rank(root, val);
	}
	inline int get_val(avltree& p, int rank)
	{
		if (getsize(p->ls) >= rank)
			return get_val(p->ls, rank);
		if (getsize(p->ls) + p->freq >= rank)
			return p->data;
		return get_val(p->rs, rank - getsize(p->ls) - p->freq);
	}
	inline int getval(int rank)
	{
		return get_val(root, rank);
	}
	inline int getprev(int val)
	{
		avltree ans = new avlnode(-1LL << 42), p = root;
		while (p)
		{
			if (p->data == val)
			{
				if (p->ls)
				{
					p = p->ls;
					while(p->rs)
					{
						p = p->rs;
					}
					ans = p;
				}
				break;
			}
			if (p->data<val && p->data>ans->data)
			{
				ans = p;
			}
			p = p->data < val ? p->rs : p->ls;
		}
	}
	inline int getnext(int val)
	{
		avltree ans = new avlnode(1LL << 42), p = root;
		while (p)
		{
			if (p->data == val)
			{
				if (p->rs)
				{
					p = p->rs;
					while (p->ls)
					{
						p = p->ls;
					}
					ans = p;
				}
				break;
			}
			if (p->data > val && p->data < ans->data)
			{
				ans = p;
			}
			p = p->data < val ? p->rs : p->ls;
		}
	}
};

*/