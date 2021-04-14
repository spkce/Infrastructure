#ifndef __LINK_H__
#define __LINK_H__

namespace Infra
{

struct Node;
/**
 * @brief 链表节点管理类
*/
struct LinkManager
{
	LinkManager();
	virtual ~LinkManager();
	struct Node* getNode();
	struct Node* begin;
	struct Node* end;
	unsigned int iNode;
};

/**
 * @brief 双向链表类
*/
class CLink
{
public:
	CLink();
	virtual ~CLink();
	/**
	* @brief 向链表插入一个负载
	* @param payload 负载指针
	* @param pos 插入的位置
	* @param return 成功：目前节点的个数；失败，-1
	**/
	int insert(void* payload, unsigned int pos);
	/**
	* @brief 从链表中移除一个负载
	* @param payload 负载指针
	* @param pos 移除的位置
	* @param return 目前节点的个数
	**/
	int remove(void** payload, unsigned int pos);
	/**
	* @brief 在链表末尾增加一个负载
	* @param payload 负载指针
	* @param return 目前节点的个数
	**/
	int rise(void* payload);
	/**
	* @brief 在链表末尾删除一个负载
	* @param payload 负载指针
	* @param return 目前节点的个数
	**/
	int reduce(void** playload);
	/**
	* @brief 获取链表中的一个负载，不进行删除
	* @param pos 位置
	* @param return 负载指针
	**/
	void* get(unsigned int pos);
	/**
	* @brief 获取链表长度
	* @param return 链表长度
	**/
	unsigned int linkSize() const;
	/**
	* @brief 释放链表
	**/
	void clear();
private:
	Node* find(unsigned int pos);
	void release();
	struct LinkManager m_manager;
};

}//Infra

#endif //__LINK_H__
