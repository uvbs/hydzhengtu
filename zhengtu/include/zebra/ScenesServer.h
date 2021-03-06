/**
 * \brief zebra项目场景服务器，游戏绝大部分内容都在本实现
 */


#ifndef _SCENESSERVER_H
#define _SCENESSERVER_H

#include <zebra/srvEngine.h>

#include <fstream>

#include <set>
#include <hash_set>

#include <luabind/luabind.hpp>

#include <zebra/csBox.h>

struct userDupMap;

//SKY 包袱的宽和高
#define PACK_WIDTH	7
#define PACK_HEIGHT	35

//SKY  仓库的宽和高
#define SAVEBOX_WIDTH 7
#define SAVEBOX_HEIGHT 50

//sky 特殊处理技能的ID
#define SKILLID_IMMOLATE	24	//献祭
#define SKILLID_SOUL		88	//灵魂状态
//sky 被攻击有几率移出的技能
#define SKILLID_FEAR		53	//恐惧
#define SKILLID_FROZEN		34	//冰冻
#define SKILLID_LIME		64	//石灰粉
#define SKILLID_REPENTANCE	57	//忏悔

// sky 控制技能系统免疫时间(秒)
const DWORD  ImmuneTime = 5;

namespace Cmd
{
	struct stCowBoxUserCmd;
	struct stTurnUserCmd;
	struct stReMakUserCmd;
	struct stAttruByteUserCmd;		//sky 自由加点消息
	struct stMakeObjectUserCmd;		//sky 宝石镶嵌消息
}


#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0,ch1,ch2,ch3)                              \
  ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |       \
   ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */

#define MAP_MAGIC (MAKEFOURCC('X','M','P','S'))
#define MAP_VERSION 6 


class stRangMap;	//sky 动态地图配置类

class zObject;
class Equips;
class Package;
class ObjectPack;
class EquipPack;
class MainPack;
class StorePack;
class PetPack;
class Packages;
class GlobalObjectIndex;
class UserObjectCompare;
class UserObjectExec;
class UserObjectM;

class SceneEntryPk;
class SceneUser;
class SceneTask;
class SceneTaskManager;
class QuestList;
class Vars;
class Condition;
class Action;

class GuardNpc;
class CartoonPet;
class Dice;

class Scene;
class zSkill;
class zSkillB;
class ScenePet;

class SceneEntryPk;
class SceneNpc;

class NpcAIController;

extern WORD max_level;
extern QWORD max_exp;
extern WORD trun_point_rate;
extern WORD trun_skill_rate;

//sky 极品掉落几率全局申明
extern WORD g_blue_changce;
extern WORD g_green_changce;
extern WORD g_purple_changce;
extern WORD g_orange_changce;

//sky 打坐回复系数
extern WORD recoverRate;

//sky 有益技能列表
extern std::vector<DWORD> UseableMagicList;

//sky 物品冷却配置数据
typedef struct stItemTypeCoolTiem
{
	stItemTypeCoolTiem()
	{
		ItemType = 0;
		CoolTime = 0;
	}

	WORD ItemType; //sky 物品类型
	DWORD CoolTime; //sky 冷却时间
};

typedef struct stItemIdCoolTime
{
	stItemIdCoolTime()
	{
		ItemID = 0;
		CoolTime = 0;
	}

	DWORD ItemID;	//sky 物品ID
	DWORD CoolTime; //sky 冷却时间
};

struct stXmlItemCoolTime
{
	stXmlItemCoolTime()
	{
		CoolTimeType = 0;
		nCoolTime = 0;
		TypeCoolTime.clear();
		IdCoolTime.clear();
	}

	WORD CoolTimeType; //sky 冷却类型
	DWORD nCoolTime;	//sky 默认冷却时间
	std::vector<stItemTypeCoolTiem> TypeCoolTime;
	std::vector<stItemIdCoolTime> IdCoolTime;
};
extern std::vector<stXmlItemCoolTime> vXmlItemCoolTime;
//sky 冷却配置 end

//[Shx Add 套装配置信息]
#pragma pack(1)
struct st_SuitEffect
{
//************** HEAD *****************
	BYTE eRequire;		//激活条件
	BYTE eKey;			//激活属性			
	WORD eValue;		//值	
};
#pragma pack()

#define  MAX_SUITEFFECT		6

struct stxml_SuitAttribute
{
	WORD id;		//套装ID;
	BYTE count;		//成员数量;
	BYTE eCount;	//属性数量;

	char	Name[MAX_NAMESIZE];				//套装名称;
	DWORD	MemberList[MAX_SUIT_NUM];		//成员列表;
	std::vector<st_SuitEffect> EffectList;

	stxml_SuitAttribute()
	{
		ZeroMemory(Name, MAX_NAMESIZE);
		ZeroMemory(MemberList, sizeof(MemberList));
		id = 0;
		count = 0;
		eCount = 0;
		EffectList.clear();
	}


	bool IsApart(DWORD id)		//物品是否套装一部分;
	{
		if(id > 0)
		{
			for(int i = 0; i < count; i ++)
			{
				if(MemberList[i] == id)
				{
					return true;
				}
			}
		}
		return false;
	}
};
extern std::vector<stxml_SuitAttribute> vXmlSuitAttribute;

struct stIncrementBySuit
{
	WORD x_str;			//	力量
	WORD x_inte;		//	智力
	WORD x_dex;			//	敏捷
	WORD x_spi;			//	精神
	WORD x_con;			//	耐力

	WORD x_pdam;		//	增加物理攻击力
	WORD x_pdef;		//	增加物理防御力
	WORD x_mdam;		//	增加魔法攻击力
	WORD x_mdef;		//	增加魔法防御力

	WORD x_p_pdam;		//	增加物理攻击力%
	WORD x_p_pdef;		//	增加物理防御力%
	WORD x_p_mdam;		//	增加魔法攻击力%
	WORD x_p_mdef;		//	增加魔法防御力%

	WORD x_akspeed;		//	攻击速度
	WORD x_mvspeed;		//	移动速度
	WORD x_atrating;	//	命中率
	WORD x_akdodge;		//	躲避率
	WORD x_hpr;			//	生命值恢复
	WORD x_mpr;			//	法术值恢复
	WORD x_bang;		//	爆击
	WORD x_hpleech;		//	x%吸收生命值y
	WORD x_mpleech;		//	x%吸收法术值y
	WORD x_incgold;		//	增加金钱掉落x%
	WORD x_doublexp;	//	x%双倍经验    

	WORD x_giddy;		//	眩晕
	WORD x_coma;		//	昏迷
	WORD x_halt;		//	定身
	WORD x_dread;		//	恐惧
	WORD x_slowdown;	//	减速
	WORD x_banish;		//	放逐

	WORD x_giddy_def;	//	防眩晕
	WORD x_coma_def;	//	防昏迷
	WORD x_halt_def;	//	防定身
	WORD x_dread_def;	//	防恐惧
	WORD x_slowdown_def;//	防减速
	WORD x_banish_def;	//	防放逐

	stIncrementBySuit(){	clear();	}
	void clear(){	ZeroMemory(this, sizeof(*this));	}
};
//End Shx


//sky 物品冷却配置数据(发送给客户端使用)
extern std::vector<BYTE> CoolTimeSendData;

namespace luabind
{
  namespace detail {
    template<class T>
    struct delete_s;
  }
}

//sky 为拉适应他的老的队伍形式所以只好在做个凌驾于用户之上的队伍管理列表
struct Team;
class TeamManager;

extern std::set<DWORD> dupMapList;//副本地图列表

/**
 * \brief 点结构
 */
struct Point
{
  /// 名称
  char name[MAX_NAMESIZE];

  /// 位置
  zPos pos;

  /**
   * \brief 构造函数初始化
   */
  Point() : pos()
  {
    bzero((void *)name,sizeof(name));
  }

  /**
   * \brief 拷贝构造
   * \param point 点
   */
  Point(const Point &point)
  {
    bcopy((void *)point.name,name,sizeof(name),sizeof(name));
    pos = point.pos;
  }

  /**
   * \brief 重载操作符,拷贝点
   * \param point 被拷贝的点
   */
  Point & operator= (const Point &point)
  {
    bcopy((void *)point.name,name,sizeof(name),sizeof(name));
    pos = point.pos;
	return *this;
  }
};

// [ranqd] 区域类型定义
struct zZone{
	zPos pos;// 左上角的点
	int  width; // 区域宽度
	int  height; // 区域高度

	zZone()
	{
		pos = zPos(0,0);
		width = 0;
		height = 0;
	}
	bool IsInZone( zPos p ) // 判断点p是否在此区域内
	{
		return  p.x >= pos.x && p.x < pos.x + width && p.y >= pos.y && p.y < pos.y + height;
	}
	zPos GetRandPos() // 返回该区域内随机一点
	{
		zPos p;
		p.x = randBetween(pos.x, pos.x + width - 1);
		p.y = randBetween(pos.y, pos.y + height - 1);
		return p;
	}
	bool IsEmpty()
	{
		return width == 0 && height == 0;
	}
};
// [ranqd] 带地图名称的区域
struct zPoint
{
	/// 地图名称
	char name[MAX_NAMESIZE];

	/// 地图区域
	zZone pos;

	/**
	* \brief 构造函数初始化
	*/
	zPoint() : pos()
	{
		bzero(name,sizeof(name));
	}

	/**
	* \brief 拷贝构造
	* \param point 点
	*/
	zPoint(const zPoint &point)
	{
		bcopy((void *)point.name,name,sizeof(name),sizeof(name));
		pos = point.pos;
	}

	/**
	* \brief 重载操作符,拷贝点
	* \param point 被拷贝的点
	*/
	zPoint & operator= (const zPoint &point)
	{
		bcopy((void *)point.name,name,sizeof(name),sizeof(name));
		pos = point.pos;
		return *this;
	}
};
/**
 * \brief 传点定义
 */
struct WayPoint
{
  friend class WayPointM;
  private:

    /// 目的坐标点集合
 //   std::vector<Point> dest;
    // [ranqd] 传送点更改为区域型
	std::vector<zPoint> dest;

    /// 目的坐标点数目
    WORD destC;
  public:
    /// 传送坐标点集合
 // std::vector<zPos> point;
	// [ranqd] 传送点更改为区域型
	std::vector<zZone> point;
    /// 传送坐标点数目
    WORD pointC;

    bool init(zXMLParser *parser,const xmlNodePtr node,DWORD countryid);
    WayPoint();
    const Point getRandDest();
    const zPos getRandPoint();
};

class WayPointM
{
  private:
    /// 传点集合
    std::vector<WayPoint> wps;
  public:
    bool addWayPoint(const WayPoint &wp);
    WayPoint *getWayPoint(const zPos &pos);
    WayPoint *getWayPoint(const char *filename);
    WayPoint *getRandWayPoint();
};

typedef std::vector<zPos> zPosIndex;

/**
 * \brief 区域的定义
 * 包括开头、中心、结束点的坐标和区域内非阻挡点索引
 *
 */
struct zRegion
{
  zPos s;        /// 左上点
  zPos c;        /// 中心点
  zPos e;        /// 右下点
  zPosIndex index;  /// 非阻挡点索引
  /**
   * \brief 构造函数
   *
   */
  zRegion()
  {
  }
  /**
   * \brief 拷贝构造函数
   *
   */
  zRegion(const zRegion &reg)
  {
    s = reg.s;
    c = reg.c;
    e = reg.e;
    index = reg.index;
  }
  /**
   * \brief 赋值操作符号
   *
   */
  zRegion & operator= (const zRegion &reg)
  {
    s = reg.s;
    c = reg.c;
    e = reg.e;
    index = reg.index;
    return *this;
  }

  /**
   * \brief 判断坐标是否在区域范围之内
   *
   */
  const bool isIn(const zPos &pos) const
  {
    return pos >= s && pos <= e;
  }

};

struct zObject:zEntry
{
  friend class GlobalObjectIndex;
  public:
    t_Object data;
    zObjectB *base;

    //only for RebuildObject::make,it's ugly,but .....sigh
    WORD* _p1[5];
    WORD* _p2[18];
    
    static zObject *create(zObjectB *objbase,DWORD num=1,BYTE level = 0);
    static void destroy(zObject* ob);
    static void logger(QWORD createid,DWORD objid,char *objname,DWORD num,DWORD change,DWORD type,DWORD srcid,char *srcname,DWORD dstid,char *dstname,const char *action,zObjectB *base,BYTE kind,BYTE upgrade);
    static DWORD RepairMoney2RepairGold(DWORD dwObjectRepair);
    static zObject *create(zObject *objsrc);
    static zObject *load(const SaveObject *o);
    bool getSaveData(SaveObject *save);
    int foundSocket();
    bool canMail();
    void checkBind();
    
    zCountryMaterialB* canContribute(); // 是否能被捐献
    DWORD getMaterialKind(); // 获得原料类别: 0,普通物资，1,丝线,2,矿石,3,矿产,4,木材，5,皮毛,6,草药

    const stObjectLocation &reserve() const;
    void restore(const stObjectLocation &loc);
      
    union
    {
      struct 
      {
        DWORD createtime;
        DWORD dwCreateThisID;  
      };
      QWORD createid;
    };
    // */
  private:
    friend class Package;
    friend class Packages;
    friend class MainPack;
    friend class zSceneObject;
    friend class luabind::detail::delete_s<zObject>;

    zObject();
    ~zObject();

    bool free() const;
    void free(bool flag);

    void fill(t_Object& d);    
    void generateThisID();
    bool inserted;

	//[Shx Add 检查该物品是否套装组成]
	void MakeSuit(zObjectB *objbase);
	void FillSuit();
	void FillSuitPPT(int nIndex);
};

struct zSceneObject:public zSceneEntry
{
  static zSceneObject *create(zObject *obj,const zRTime &ct)
  {
    if (obj==NULL) return NULL;
    zSceneObject *ret=new zSceneObject(obj,ct);
    return ret;
  }
  /**
   * \brief 检查地上物品是否过期
   * 过期的地上物品会消失掉
   * \param ct 当前时间
   * \return 是否过期
   */
  bool checkOverdue(const zRTime &ct) const
  {
    return ct >= this->ct;
  }

  bool checkProtectOverdue(const zRTime &ct)
  {
    if (ownerID == 0)
    {
      return false;
    }
    if (ct > protectTime)
    {
      protectTime = 0;
      ownerID = 0;
      return true;
    }
    return false;
  }

  void setOwner(DWORD dwID,int protime=10)
  {
    if (dwID)
    {
      ownerID = dwID;
      zRTime ctv;
      protectTime = ctv;
      this->protectTime.addDelay(protime * 1000);
    }
  }

  void setOverDueTime(zRTime &ct_1)
  {
    ct = ct_1;
  }
  DWORD getOwner()
  {
    return ownerID;
  }

  ~zSceneObject()
  {
    if (o) o->free();
    zObject::destroy(o);
  }

  void clear()
  {
    o=NULL;
  }

  zObject *getObject()
  {
    return o;
  }

  // sky 重新设置保护时间
  void setprotectTime( int i )
  {
	  protectTime.addDelay( i * 1000 );
  }

  // sky 强行删除物品保护
  bool DelProtectOverdue()
  {
	  if( ownerID == 0)
		  return false;

	  protectTime = 0;
	  ownerID = 0;
	  return true;
  }

  private:
  DWORD ownerID;
  zRTime protectTime;
  zObject *o;
  zRTime ct;
  zSceneObject(zObject *obj,const zRTime &ct):zSceneEntry(SceneEntry_Object),ct(ct)
  {
    this->ct.addDelay(600 * 1000);
    o=obj;
    ownerID=0;
    id = obj->data.qwThisID;
    tempid = obj->base->id;
    strncpy_s(name,obj->data.strName,MAX_NAMESIZE);
  }
};

#define DECLARE(prop,type) type get_##prop() const {return prop;}
#define RESET(prop) prop = 0;
class Equips 
{
public:
  Equips()
  {
    reset();
  }
  
  void reset()
  {
	  RESET(maxhp)          // 最大生命值
		  RESET(maxmp)          // 最大法术值
		  RESET(maxsp)          // 最大体力值

		  RESET(pdamage)          // 最小攻击力
		  RESET(maxpdamage)        // 最大攻击力
		  RESET(mdamage)          // 最小法术攻击力
		  RESET(maxmdamage)        // 最大法术攻击力
		  RESET(appendminpet)           // 宠物增强最小值
		  RESET(appendmaxpet)        // 宠物增强最大值

		  RESET(pdefence)        // 物防
		  RESET(mdefence)        // 魔防
		  RESET(damagebonus)      // 伤害加成
		  RESET(damage)        // 增加伤害值x％

		  RESET(akspeed)        // 攻击速度
		  RESET(mvspeed)        // 移动速度
		  RESET(atrating)        // 命中率
		  RESET(akdodge)        // 躲避率

		  RESET(str)            // 力量
		  RESET(inte)            // 智力
		  RESET(dex)            // 敏捷
		  RESET(spi)            // 精神
		  RESET(con)            // 体质

		  RESET(hpr)          // 生命值恢复
		  RESET(mpr)            // 法术值恢复
		  RESET(spr)            // 体力值恢复

		  RESET(holy)            //神圣一击
		  RESET(bang)            //重击率
		  RESET(pdam)           // 增加物理攻击力
		  RESET(pdef)            // 增加物理防御力
		  RESET(mdam)            // 增加魔法攻击力
		  RESET(mdef)            // 增加魔法防御力

		  RESET(poisondef)         //抗毒增加
		  RESET(lulldef)         //抗麻痹增加
		  RESET(reeldef)         //抗眩晕增加
		  RESET(evildef)         //抗噬魔增加
		  RESET(bitedef)         //抗噬力增加
		  RESET(chaosdef)         //抗混乱增加
		  RESET(colddef)         //抗冰冻增加
		  RESET(petrifydef)       //抗石化增加
		  RESET(blinddef)         //抗失明增加
		  RESET(stabledef)         //抗定身增加
		  RESET(slowdef)         //抗减速增加
		  RESET(luredef)         //抗诱惑增加

		  RESET(poison)         //中毒增加
		  RESET(lull)           //麻痹增加
		  RESET(reel)           //眩晕增加
		  RESET(evil)           //噬魔增加
		  RESET(bite)          //噬力增加
		  RESET(chaos)           //混乱增加
		  RESET(cold)           //冰冻增加
		  RESET(petrify)         //石化增加
		  RESET(blind)           //失明增加
		  RESET(stable)         //定身增加
		  RESET(slow)           //减速增加
		  RESET(lure)           //诱惑增加
		  RESET(hpleech.odds) 
		  RESET(hpleech.effect) 
		  RESET(mpleech.odds)       //x%吸收生命值y
		  RESET(mpleech.effect)     //x%吸收法术值y

		  RESET(hptomp)          //转换生命值为法术值x％
		  RESET(dhpp)           //物理伤害减少x%  
		  RESET(dmpp)          //法术伤害值减少x%    

		  RESET(incgold)        //增加金钱掉落x%
		  RESET(doublexp)        //x%双倍经验    
		  RESET(mf)             //增加掉宝率x%

		  //五行套装相关属性
		  RESET(dpdam)      //物理伤害减少%x
		  RESET(dmdam)      //法术伤害减少%x
		  RESET(bdam)        //增加伤害x%
		  RESET(rdam)        //伤害反射%x
		  RESET(ignoredef)    //%x忽视目标防御

		  //    RESET(aftype)
		  aftype = FIVE_NONE;
	      RESET(afpoint)
		  //    RESET(dftype)
		  dftype = FIVE_NONE;
		  RESET(dfpoint)
		  RESET(maxhprate)
		  RESET(maxmprate)

		  RESET(Freedom.str_Attribute)
		  RESET(Freedom.inte_Attribute)
		  RESET(Freedom.dex_Attribute)
		  RESET(Freedom.spi_Attribute)
		  RESET(Freedom.con_Attribute)

		  skill.clear();
		  skills.clear();

		  gempop.clear();
  }
  
  DECLARE(maxhp,WORD)        // 最大生命值
  DECLARE(maxmp,WORD)        // 最大法术值
  DECLARE(maxsp,WORD)        // 最大体力值

  DECLARE(pdamage,WORD)      // 最小攻击力
  DECLARE(maxpdamage,WORD)    // 最大攻击力
  DECLARE(mdamage,WORD)      // 最小法术攻击力
  DECLARE(maxmdamage,WORD)    // 最大法术攻击力
  DECLARE(appendminpet,WORD)  // 最小宠物增强
  DECLARE(appendmaxpet,WORD)  // 最大宠物增强

  DECLARE(pdefence,WORD)      // 物防
  DECLARE(mdefence,WORD)      // 魔防
  DECLARE(damagebonus,BYTE)    // 伤害加成
  DECLARE(damage,BYTE)        // 增加伤害值x％
  
  DECLARE(akspeed,WORD)      // 攻击速度
  DECLARE(mvspeed,WORD)      // 移动速度
  DECLARE(atrating,WORD)      // 命中率
  DECLARE(akdodge,WORD)      // 躲避率

  DECLARE(str,WORD)          // 力量
  DECLARE(inte,WORD)         // 智力
  DECLARE(dex,WORD)         // 敏捷
  DECLARE(spi,WORD)          // 精神
  DECLARE(con,WORD)          // 体质
  
  DECLARE(hpr,WORD)          // 生命值恢复
  DECLARE(mpr,WORD)          // 法术值恢复
  DECLARE(spr,WORD)          // 体力值恢复
  
  DECLARE(holy,WORD)        //神圣一击
  DECLARE(bang,WORD)        //重击率
  DECLARE(pdam,WORD)          // 增加物理攻击力
  DECLARE(pdef,WORD)          // 增加物理防御力
  DECLARE(mdam,WORD)          // 增加魔法攻击力
  DECLARE(mdef,WORD)          // 增加魔法防御力
  
  DECLARE(poisondef,WORD)       //抗毒增加
  DECLARE(lulldef,WORD)       //抗麻痹增加
  DECLARE(reeldef,WORD)       //抗眩晕增加
  DECLARE(evildef,WORD)       //抗噬魔增加
  DECLARE(bitedef,WORD)       //抗噬力增加
  DECLARE(chaosdef,WORD)       //抗混乱增加
  DECLARE(colddef,WORD)       //抗冰冻增加
  DECLARE(petrifydef,WORD)     //抗石化增加
  DECLARE(blinddef,WORD)       //抗失明增加
  DECLARE(stabledef,WORD)       //抗定身增加
  DECLARE(slowdef,WORD)       //抗减速增加
  DECLARE(luredef,WORD)       //抗诱惑增加
    
  DECLARE(poison,WORD)       //中毒增加
  DECLARE(lull,WORD)         //麻痹增加
  DECLARE(reel,WORD)         //眩晕增加
  DECLARE(evil,WORD)         //噬魔增加
  DECLARE(bite,WORD)         //噬力增加
  DECLARE(chaos,WORD)         //混乱增加
  DECLARE(cold,WORD)         //冰冻增加
  DECLARE(petrify,WORD)       //石化增加
  DECLARE(blind,WORD)         //失明增加
  DECLARE(stable,WORD)       //定身增加
  DECLARE(slow,WORD)         //减速增加
  DECLARE(lure,WORD)         //诱惑增加

  DECLARE(hptomp,BYTE)       //转换生命值为法术值x％
  DECLARE(dhpp,BYTE)         //物理伤害减少x%  
  DECLARE(dmpp,BYTE)         //法术伤害值减少x%    

  DECLARE(incgold,BYTE)       //增加金钱掉落x%
  DECLARE(doublexp,BYTE)       //x%双倍经验    
  DECLARE(mf,BYTE)         //增加掉宝率x%

  //五行套装相关属性
  DECLARE(dpdam,BYTE)      //物理伤害减少%x
  DECLARE(dmdam,BYTE)      //法术伤害减少%x
  DECLARE(bdam,BYTE)        //增加伤害x%
  DECLARE(rdam,BYTE)        //伤害反射%x
  DECLARE(ignoredef,BYTE)    //%x忽视目标防御
  DECLARE(maxhprate,BYTE)    //按百分比增加最大hp
  DECLARE(maxmprate,BYTE)    //按百分比增加最大mp

  //sky 新增自由加点的属性 
  WORD get_Freedom_str()		const { return Freedom.str_Attribute;}
  WORD get_Freedom_inte()		const { return Freedom.inte_Attribute;}
  WORD get_Freedom_dex()		const { return Freedom.dex_Attribute;}
  WORD get_Freedom_spi()		const { return Freedom.spi_Attribute;}
  WORD get_Freedom_con()		const { return Freedom.con_Attribute;}

  BYTE get_hpleech_odds()    const  {return hpleech.odds;}		 
  WORD get_hpleech_effect()  const  {return hpleech.effect;}	 //x%吸收生命值y
  BYTE get_mpleech_odds()    const  {return mpleech.odds;}      
  WORD get_mpleech_effect()  const  {return mpleech.effect;}     //x%吸收法术值y

   /**
   * \brief sky 获取宝石属性
   * \return 攻击五行类型
   */
  WORD get_gem_str() const   //获取宝石的力量
  {
	  WORD str;
	  const_iterator1 it;

	  for(it = gempop.begin(); it!=gempop.end(); it++)
	  {
		  if( it->GemID != INVALID_HOLE )
			  str += it->gem_str;
	  }	

	  return str;
  }

  WORD get_gem_inte() const   //获取宝石的智力
  {
	  WORD inte;
	  const_iterator1 it;

	  for(it = gempop.begin(); it!=gempop.end(); it++)
	  {
		  if( it->GemID != INVALID_HOLE )
			  inte += it->gem_inte;
	  }	

	  return inte;
  }

  WORD get_gem_dex() const   //获取宝石的敏捷
  {
	  WORD dex;
	  const_iterator1 it;

	  for(it = gempop.begin(); it!=gempop.end(); it++)
	  {
		  if( it->GemID != INVALID_HOLE )
			  dex += it->gem_dex;
	  }	

	  return dex;
  }

  WORD get_gem_spi() const   //获取宝石的精神
  {
	  WORD spi;
	  const_iterator1 it;

	  for(it = gempop.begin(); it!=gempop.end(); it++)
	  {
		  if( it->GemID != INVALID_HOLE )
			  spi += it->gem_spi;
	  }	

	  return spi;
  }

  WORD get_gem_con() const   //获取宝石的耐力
  {
	  WORD con;
	  const_iterator1 it;

	  for(it = gempop.begin(); it!=gempop.end(); it++)
	  {
		  if( it->GemID != INVALID_HOLE )
			  con += it->gem_con;
	  }	

	  return con;
  }

	WORD	gem_atk;	//物理攻击力
	WORD	gem_mkt;

  WORD get_gem_def() const   //获取宝石的物理防御
  {
	  WORD def;
	  const_iterator1 it;

	  for(it = gempop.begin(); it!=gempop.end(); it++)
	  {
		  if( it->GemID != INVALID_HOLE )
			  def += it->gem_def;
	  }	

	  return def;
  }

  WORD get_gem_mdef() const   //获取宝石的魔法防御
  {
	  WORD mdef;
	  const_iterator1 it;

	  for(it = gempop.begin(); it!=gempop.end(); it++)
	  {
		  if( it->GemID != INVALID_HOLE )
			  mdef += it->gem_mdef;
	  }	

	  return mdef;
  }

  WORD get_gem_dhpp() const   //获取宝石的物理免伤
  {
	  WORD dhpp;
	  const_iterator1 it;

	  for(it = gempop.begin(); it!=gempop.end(); it++)
	  {
		  if( it->GemID != INVALID_HOLE )
			  dhpp += it->gem_dhpp;
	  }	

	  return dhpp;
  }

  WORD get_gem_dmpp() const   //获取宝石的魔法免伤
  {
	  WORD dmpp;
	  const_iterator1 it;

	  for(it = gempop.begin(); it!=gempop.end(); it++)
	  {
		  if( it->GemID != INVALID_HOLE )
			  dmpp += it->gem_dmpp;
	  }	

	  return dmpp;
  }

  WORD get_gem_atk() const   //获取宝石的物理攻击力
  {
	  WORD atk;
	  const_iterator1 it;

	  for(it = gempop.begin(); it!=gempop.end(); it++)
	  {
		  if( it->GemID != INVALID_HOLE )
			  atk += it->gem_atk;
	  }	

	  return atk;
  }

  WORD get_gem_mtk() const   //获取宝石的魔法攻击力
  {
	  WORD mtk;
	  const_iterator1 it;

	  for(it = gempop.begin(); it!=gempop.end(); it++)
	  {
		  if( it->GemID != INVALID_HOLE )
			  mtk += it->gem_mkt;
	  }	

	  return mtk;
  }

  /**
   * \brief 获取攻击五行
   * \return 攻击五行类型
   */
  DWORD getAttFive() const {return aftype;}

  /**
   * \brief 获取攻击五行点数
   * \return 五行点数
   */
  DWORD getAttFivePoint() const {return afpoint;}

  /**
   * \brief 获取防御五行
   * \return 防御五行类型
   */
  DWORD getDefFive() const {return dftype;}

  /**
   * \brief 获取防御五行点数
   * \return 五行点数
   */
  DWORD getDefFivePoint() const {return dfpoint;}
  
  /**
   * \brief 获取装备对单个技能的加成
   *
   * \param id: 技能id
   * \return 加成等级
   */  
  DWORD getMaxSkill(WORD id) const
  {
    const_iterator it = skill.find(id);
    if (it != skill.end()) {
      return it->second;
    }
    
    return 0;    
  }
  
  /**
   * \brief 获取装备对某系技能的加成
   *
   * \param id: 技能系别
   * \return 加成等级
   */  
  DWORD getMaxSkills(WORD id) const
  {
    const_iterator it = skills.find(id);
    if (it != skills.end()) {
      return it->second;
    }
    
    return 0;    
  }

private:
  friend class EquipPack;
  DWORD maxhp;          // 最大生命值
  DWORD maxmp;          // 最大法术值
  DWORD maxsp;          // 最大体力值

  DWORD pdamage;        // 最小攻击力
  DWORD maxpdamage;      // 最大攻击力
  DWORD mdamage;        // 最小法术攻击力
  DWORD maxmdamage;      // 最大法术攻击力
  WORD appendminpet;         // 给宠召唤兽能力增强最小值
  WORD appendmaxpet;         // 给宠召唤兽能力增强最大值

  WORD pdefence;        // 物防
  WORD mdefence;        // 魔防
  BYTE damagebonus;      // 伤害加成
  BYTE damage;        // 增加伤害值x％
  
  WORD akspeed;        // 攻击速度
  WORD mvspeed;        // 移动速度
  WORD atrating;        // 命中率
  WORD akdodge;        // 躲避率

  WORD str;  // 力量
  WORD inte;  // 智力
  WORD dex;  // 敏捷
  WORD spi;  // 精神
  WORD con;  // 体质
  
  
//      WORD fivetype;  // 五行属性
//      WORD fivepoint; // 五行属性
  
  WORD hpr;  // 生命值恢复
  WORD mpr;  // 法术值恢复
  WORD spr;  // 体力值恢复
  
  WORD holy;  //神圣一击  
  WORD bang;  //重击率
  WORD pdam;  // 增加物理攻击力
  WORD pdef;  // 增加物理防御力
  WORD mdam;  // 增加魔法攻击力
  WORD mdef;  // 增加魔法防御力
  
  WORD poisondef; //抗毒增加
  WORD lulldef; //抗麻痹增加
  WORD reeldef; //抗眩晕增加
  WORD evildef; //抗噬魔增加
  WORD bitedef; //抗噬力增加
  WORD chaosdef; //抗混乱增加
  WORD colddef; //抗冰冻增加
  WORD petrifydef; //抗石化增加
  WORD blinddef; //抗失明增加
  WORD stabledef; //抗定身增加
  WORD slowdef; //抗减速增加
  WORD luredef; //抗诱惑增加
    
  WORD poison; //中毒增加
  WORD lull; //麻痹增加
  WORD reel; //眩晕增加
  WORD evil; //噬魔增加
  WORD bite; //噬力增加
  WORD chaos; //混乱增加
  WORD cold; //冰冻增加
  WORD petrify; //石化增加
  WORD blind; //失明增加
  WORD stable; //定身增加
  WORD slow; //减速增加
  WORD lure; //诱惑增加
  
  struct leech
  {
    BYTE odds;    //x
    WORD effect;  //y
  };
  leech hpleech; //x%吸收生命值y
  leech mpleech; //x%吸收法术值y
  
  BYTE hptomp; //转换生命值为法术值x％
  BYTE dhpp; //物理伤害减少x%  
  BYTE dmpp; //法术伤害值减少x%    

  BYTE incgold; //增加金钱掉落x%
  BYTE doublexp; //x%双倍经验    
  BYTE mf; //增加掉宝率x%

  union {
    BYTE _five_props[5];
    struct {
      //五行套装相关属性
      BYTE dpdam; //物理伤害减少%x
      BYTE dmdam; //法术伤害减少%x
      BYTE bdam; //增加伤害x%
      BYTE rdam; //伤害反射%x
      BYTE ignoredef; //%x忽视目标防御
    };
  };
  
  WORD aftype;  
  WORD afpoint;
  WORD dftype;
  WORD dfpoint;
  
  typedef std::map<WORD,WORD> SKILL;
  typedef SKILL::const_iterator const_iterator;
  SKILL skill;  //单个技能加成
  SKILL skills; //全系技能加成 
  BYTE maxhprate;  //按百分比增加最大hp
  BYTE maxmprate;  //按百分比增加最大mp

  struct Freedom_Attribute {
	  WORD str_Attribute;		// 力量已经加过的自由点
	  WORD inte_Attribute;	// 智力已经加过的自由点
	  WORD dex_Attribute;		// 敏捷已经加过的自由点
	  WORD spi_Attribute;		// 精神已经加过的自由点
	  WORD con_Attribute;		// 体质已经加过的自由点

  } Freedom;

  //sky 新增宝石属性结构
  typedef std::vector<GemPop> GEMPOP;
  typedef GEMPOP::const_iterator const_iterator1;
  GEMPOP gempop;
};  

struct PackageCallback
{
  virtual bool exec(zObject * o)=0;
  virtual ~PackageCallback(){};
};

class Package:private zNoncopyable
{
  protected:
    zObject** container;

    virtual bool add(zObject *object,bool find);
    virtual bool remove(zObject *object);
    virtual bool checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y);    
  public:
    Package(WORD type,DWORD id,WORD w,WORD h);
    virtual ~Package();

    virtual bool getObjectByZone(zObject **ret,WORD x,WORD y);
    virtual bool getObjectByID(zObject **ret,DWORD id);
    virtual void execEvery(PackageCallback &callback);

    virtual WORD space() const;
    virtual WORD size() const;
    WORD type() const;
    DWORD id() const;
    
    void setSpace(WORD);
  private:
    friend class Packages;
    
    virtual void removeAll();  
    bool find_space(WORD &x,WORD &y) const;
    virtual int position(WORD x,WORD y) const;

    WORD _type;
    DWORD _id;
    WORD _width;
    WORD _height;

    WORD _space;
    WORD _size;

};

#if 0
class MultiPack:public Package
{
  protected:
    WORD width;
    WORD height;
    std::vector<std::vector<zObject *> >grid;
    std::set<zObject *> allset;
  public:
    MultiPack(WORD type,DWORD id,WORD width,WORD height);
    virtual ~MultiPack();
    virtual bool add(zObject *object,bool find);
    virtual void remove(zObject *object);
    virtual void removeAll();
    virtual bool checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y);
    virtual bool getObjectByZone(zObject **ret,WORD x,WORD y,WORD width,WORD height);
    virtual void execEvery(PackageCallback &callback);
    virtual bool findSpace(WORD owidth,WORD oheight,WORD &x,WORD &y);
    std::set<zObject *> &getAllset()
    {
      return allset;
    }
    
    WORD get_width() const 
    {
      return width;
    }
    
    WORD get_height() const 
    {
      return height;
    }

};
#endif

class ObjectPack : public Package
{
public:
  ObjectPack(zObject* ob,int type = Cmd::OBJECTCELLTYPE_PACKAGE,bool consume = true);
  ~ObjectPack();
  
  bool empty() const 
  {
    return space() == size();
  }
  
  zObject* object()
  {
    return _ob;
  }

  void consume_dur_by(SceneUser* user,const zRTime& current);
  bool can_input()
  {
    if (!_ob || _ob->data.dur == 0) {
      return false;
    }
    return true;
  }
  
private:  
  bool checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y);
  //bool add(zObject *object,bool find);
  
  zObject* _ob;
  bool _cosume_by_time;
  Timer _one_min;
};

    
class EquipPack : public Package
{
  public:

    enum EQUIPNO{    
      BODY,
      HEAD,
      HANDL,
      HANDR,
      BELT,
      NECKLACE,//5
      
      RINGL,
      RINGR,
      BANGL,
      BANGR,
      BOOTS,//10
      
      OTHERS, 
      OTHERS1, 
      OTHERS2, 
      OTHERS3,
      OTHERS4, //15  
    };
    
    enum PACKNO{
      R_PACK,
      L_PACK,
      R_MAKE,
      L_MAKE,
    };
    EquipPack(SceneUser* user);
    ~EquipPack();
    
    DWORD doubleexp_obj;
    DWORD doubleexp_obj_time;
    DWORD family_obj_times;
	DWORD family_obj_time;
    DWORD tong_obj_times;
    DWORD tong_obj_time;
    DWORD king_obj_times;
    DWORD king_obj_time;
    const Equips &getEquips() const;
    ObjectPack* pack(PACKNO no) const;
    zObject* equip(EQUIPNO no) const;
      
    bool getObjectByZone(zObject **ret,WORD x,WORD y);
    zObject *getObjectByEquipNo(EQUIPNO no);
    zObject *getObjectByEquipPos(DWORD no);

    bool costAttackDur(SceneUser *pThis);
    bool costDefenceDur(SceneUser *pThis);
    bool costAttackDurByPet(SceneUser *pThis);
    bool costDefenceDurByPet(SceneUser *pThis);
    void restituteDurability(SceneUser *pThis,const zRTime &ct);
    void updateDurability(SceneUser *pThis,DWORD value);
    DWORD fullAllEquiped(char *buf);

    bool needRecalc;
    int effectCount;

    bool skillReduceObject(SceneUser* pThis,DWORD kind,WORD num);
    bool skillCheckReduceObject(SceneUser* pThis,DWORD id,WORD num);
    bool canChangeColor();
    void calcAll();
    int reduceDur(SceneUser *pThis,DWORD which,DWORD type,DWORD num,bool needFresh=true,bool needCal=true);
    bool isTonic();

    void obtain_exp(SceneUser* user,DWORD exp,bool force = false);

		//装备包裹是否为空
	bool isEmpty();
  private:
    bool add(zObject *object,bool find);
    bool remove(zObject *object);
    bool checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y);

	//sky 检测物品的装备职业是否符合当前玩家的职业                                                                     */
	bool IsJobEquip(DWORD useJob, DWORD EquipType);

    virtual int position(WORD x,WORD y) const;
    
    bool reduceDur(SceneUser *pThis,DWORD which);
    inline void freshDurability(SceneUser *pThis,zObject *o);
    inline void freshUserData(SceneUser *pThis);

    bool process_extra_add(zObject* ob);
    bool process_extra_remove(zObject* ob);
    bool process_extra_get(zObject** ob,WORD x,WORD y);
    
    SceneUser* owner;
    ObjectPack** packs;
    Equips equips;
    const static int _poses[60];

};

#define MAX_TAB_NUM 5
#define MIN_TAB_NUM 2

class MainPack:public Package
{
  public:
    enum {
		//sky 设置包袱的格子有效区域
		WIDTH = PACK_WIDTH,
		HEIGHT = PACK_HEIGHT,
    };
    
    MainPack();
    ~MainPack();

	BYTE TabNum; //sky 玩家可以使用的包裹页数
    
    bool getObjectByZone(zObject **ret,WORD x,WORD y);
    zObject * getGold();
    DWORD getGoldNum();
    
    bool skillReduceObject(SceneUser* pThis,DWORD kind,DWORD num);

  private:
    bool add(zObject *object,bool find);
    bool remove(zObject *object);
    bool checkAdd(SceneUser *pUser,zObject *object,WORD x,WORD y);
    
    zObject *gold;
};

class StorePack : public Package
{
public:
  enum {
    W = SAVEBOX_WIDTH,
    H = SAVEBOX_HEIGHT,
  };
  
  BYTE days;
  
  StorePack();
  ~StorePack();
    
  void goldstore(SceneUser *pThis,bool notify=true);
  int load(BYTE* dest);
  int save(BYTE* dest);
  
  void notify(SceneUser* user);
  
private:
  bool checkAdd(SceneUser* pUser,zObject* object,WORD x,WORD y);
  
};

class PetPack : public Package
{
public:
  static const BYTE W = 8;
  static const BYTE H = 10;

  PetPack();
  ~PetPack();

  WORD size() const;//返回最大可用数
  void setAvailable(WORD);//设置最大可用数

  bool isEmpty() const;

private:
  WORD available;//可用的总数

  bool checkAdd(SceneUser* pUser,zObject* object,WORD x,WORD y);
};

template<typename T>
class Type2Type
{
public:
  typedef T BASE;
};

class GlobalObjectIndex:private zEntryManager< zEntryID >
{
  private:
    static GlobalObjectIndex *onlyme;
    zMutex mlock;

    GlobalObjectIndex();
    ~GlobalObjectIndex();
  public:
    static GlobalObjectIndex *getInstance();
    static void delInstance();
    void removeObject(DWORD thisid);
    bool addObject(zObject * o);
    zObject *getObjectByThisid(DWORD thisid);
};

extern GlobalObjectIndex *const goi;

class UserObjectCompare
{
  public:
    virtual bool isIt(zObject * object) =0;
};

class UserObjectExec
{
  public:
    virtual bool exec(zObject * object) =0;
};

class UserObjectM:private zEntryManager< zEntryID >
{
  public:
    typedef std::vector<DWORD > ObjID_vec;
    typedef std::vector<zObject *> Obj_vec;

    UserObjectM();
    ~UserObjectM();
    zObject * getObjectByThisID( DWORD thisid);
    zObject *getObjectByPos(const stObjectLocation &dst);
    void removeObjectByThisID(DWORD thisid);
    void removeObject(zObject * o);
    bool addObject(zObject * o);
    zObject *getObject(UserObjectCompare &comp);
    void execEvery(UserObjectExec &exec);
    
    DWORD exist(DWORD id,DWORD number,BYTE upgrade = 0,BYTE type = 0 ) const;
    int space(const SceneUser* user) const;

    zObject* getObjectByID(DWORD id,BYTE upgrade = 0,bool not_need_space = false) const;
    int reduceObjectNum(SceneUser* user,DWORD id,DWORD number,zObject*& update_ob,ObjID_vec& del_obs,BYTE upgrade = 0);    
    int addObjectNum(SceneUser* user,DWORD id,DWORD number,zObject* & orig_ob,Obj_vec& new_obs,BYTE upgrade);
    int addGreenObjectNum(SceneUser* user,DWORD id,DWORD number,zObject* & orig_ob,Obj_vec& new_obs,BYTE upgrade);
};

class Packages
{
  private:
    SceneUser *owner;

    Package** getPackage(int packs);
    
  public:
    enum {
      MAIN_PACK = 1,
      LEFT_PACK = 2,
      RIGHT_PACK = 4,
      EQUIP_PACK = 8,
      STORE_PACK = 16,
      LM_PACK = 32,
      RM_PACK = 64,
      PET_PACK = 128,
    };
    
    UserObjectM uom;
    MainPack main;
    EquipPack equip;
    StorePack store;
    PetPack petPack;

    Packages(SceneUser* user);
    ~Packages();
    void addMoney(DWORD num,const char *disc,const char *note = NULL,bool notify = true);
    void addGold(DWORD num,const char *disc,const char *note = NULL,bool notify = true,bool pack=false);
    void addTicket(DWORD num,const char *disc,const char *note = NULL,bool notify = true);
    bool checkMoney(DWORD need);
    bool checkTicket(DWORD need);
    bool checkGold(DWORD need);
    bool removeMoney(DWORD num,const char *disc);
    bool removeGold(DWORD num,const char *disc,bool need=true);
    bool removeTicket(DWORD num,const char *disc);
    
    bool moveObject(SceneUser *pUser,zObject *srcObj,stObjectLocation &dst);
    bool moveObjectToScene(zObject *o,const zPos &pos,DWORD overdue_msecs=0,const unsigned long dwID=0);
    bool removeObject(zObject *srcObj,bool notify = true,bool del = true);
    bool addObject(zObject *srcObj,bool needFind,int packs = 0);
    Package* getPackage(DWORD type,DWORD id);
    SceneUser *getOwner()
    {
      return owner;
    }
    
    template<typename T>
    void execEvery(Package* pack,T t)
    {
      typename T::BASE cb(this);
      pack->execEvery(cb);
    }
    
    template<typename T,typename P1>
    void execEvery(Package* pack,T t,P1 p1)
    {
      typename T::BASE cb(this,p1);
      pack->execEvery(cb);
    }
    
    zObject *getGold();
    DWORD getGoldNum();
};

#define AUTO_PACK Packages::MAIN_PACK |Packages::LEFT_PACK| Packages::RIGHT_PACK

struct Combination : public PackageCallback
{
public:
  Combination(SceneUser* user,zObject* get) : _user(user),_get(get),_num(0)
  { }

  virtual bool exec(zObject* o);

  int num() const
  {
    return _num;
  }

private:
  SceneUser* _user;
  zObject* _get;
  int _num;
};

struct ClearPack : public PackageCallback
{
public: 
  ClearPack(const Packages* ps) : _ps(ps)
  { }
  
  virtual bool exec(zObject* p);
private:
  const Packages* _ps;
};

struct DropFromPack : public PackageCallback
{
public:
  struct Param
  {
    Package* pack;
    int drop_num;
    zPos pos;

    Param(Package* p,int d,const zPos& s) : pack(p),drop_num(d),pos(s)
    { }
  };
  
  DropFromPack(const Packages* ps,const Param&  p) ;
  virtual bool exec(zObject* ob);
private:
  const Packages* _ps;
  Param _p;

  int _begin;
  int _pos;
};

/**
 * \brief 地图文件头结构定义
 */
struct stMapFileHeader
{
  DWORD magic;      /**< 文件标识  MAP_MAGIC */
  DWORD ver;        /**< 版本 MAP_VERSION */
  DWORD width;      /**< 宽度 */
  DWORD height;      /**< 高度 */
};

/**
 * \brief 格子定义
 */
#define TILE_BLOCK      0x01  // 阻挡点
#define TILE_MAGIC_BLOCK  0x02  // 魔法阻挡点
#define TILE_NOCREATE    0x04  // 不能建造点
#define TILE_DOOR      0x08  // 门
#define TILE_ENTRY_BLOCK  0x10  // 人物或者Npc阻挡
#define TILE_OBJECT_BLOCK  0x20  // 物品阻挡
#pragma  pack(1)
/**
 * \brief 格字数据结构
 *
 */
struct stSrvMapTile
{
  BYTE  flags;  // 格子属性
  BYTE  type;  // 格子类型
};
#pragma pack()

typedef stSrvMapTile Tile;
typedef std::vector<Tile> zTiles;

bool LoadMap(const char* pszFileName,zTiles &aTiles,DWORD & width,DWORD & height);

struct SeptGuard
{
  DWORD id; // 玩家ID
  DWORD money; // 玩家扣的押金
  bool  is_finish;

  SeptGuard()
  {
    id = 0;
    money = 0;
    is_finish = false;
  }
};

struct WarRecord
{
  DWORD type;                     // 对战类型
  DWORD relationid;               // 对方社会关系ID
  bool isAtt;                     // 是否为攻方
  bool isAntiAtt;      // 是否允许反攻（目前主要是国战使用）

  WarRecord()
  {
    type=0;
    relationid=0;
    isAtt = false;
    isAntiAtt = false;
  }
};

extern DWORD get_sell_price_by_dur(zObject*);

//sky 新增技能特殊属性枚举
enum
{
	SPEC_NULL		= 0,	//sky 空	
	SPEC_MAGIC		= 1,	//sky 魔法类
	SPEC_PHYSICS	= 2,	//sky 物理类
	SPEC_IMMUNE		= 4,	//sky 可免疫
	SPEC_REBOUND	= 8,	//sky 可被反弹
	SPEC_TREATMENT	= 16,	//sky 治疗
	SPEC_BUFF		= 32,	//sky 有益BUFF
	SPEC_DEBUFF		= 64,	//sky 减益BUFF
	SPEC_DRUG		= 128,	//sky 毒
	SPEC_OTHER		= 256,	//sky 其他
};

/**
 * \brief 技能类,定义技能的基本特性及攻击执行方法
 */
struct zSkill : zEntry
{
  static const DWORD maxUniqueID = 100000;
  public:
    t_Skill data;
    const zSkillB *base;
    const zSkillB *actionbase;

    static zSkill *create(SceneEntryPk *pEntry,DWORD id,DWORD level);
    static zSkill *createTempSkill(SceneEntryPk *pEntry,DWORD id,DWORD level);
    static zSkill *load(SceneEntryPk *pEntry,const SaveSkill *s);
    bool canUpgrade(SceneEntryPk *pEntry);
    bool setupSkillBase(SceneEntryPk *pEntry);
    bool getSaveData(SaveSkill *save);
    bool canUse();
    bool checkSkillBook(bool nextbase=false);
    bool checkSkillStudy(bool nextbase=false);
    void resetUseTime();
    void clearUseTime();
    void refresh(bool ignoredirty=false);

	//sky 新增技能特殊属性检测函数
	bool IsMagicSkill();	//sky 检测技能是否是魔法系技能
	bool IsPhysicsSkill();	//sky 检测技能是否是物理系技能
	bool IsImmuneSkill();	//sky 检测技能是否能被免疫
	bool IsReboundSkill();	//sky 检测技能是否能被反射
	bool IsTreatmentSkill();//sky 检测技能是否是治疗系
	bool IsBuffSkill();		//sky 检测技能是否是增益BUFF系
	bool IsDeBuffSkill();	//sky 检测技能是否是减益BUFF系
	bool IsDrugSkill();		//sky 检测技能是否是毒系
	bool IsOtherSkill();	//sky 检测技能是否是其他系

    ~zSkill();
    QWORD lastUseTime;
  private:
    bool dirty;
    bool istemp;
    bool needSave;
    bool inserted;
    SceneEntryPk *_entry;
    const Cmd::stAttackMagicUserCmd *curRevCmd;
    DWORD curRevCmdLen;
    zSkill();

    //static DWORD uniqueID;

// 新增加的技能方法
  public:
    bool action(const Cmd::stAttackMagicUserCmd *rev,const DWORD cmdLen);
    inline void doOperation(const SkillStatus *pSkillStatus);
    inline void doPassivenessSkill();
    inline bool showMagicToAll();
    inline bool findAttackTarget(const SkillStatus *pSkillStatus,zPos &pd,DWORD &count,SWORD rangDamageBonus=0);
    inline const zSkillB *getNewBase();
};

/**
 * \brief  技能管理器基类
 */
class SkillManager : public zEntryManager < zEntryID,zEntryTempID >
{
  protected:
    bool getUniqeID(DWORD &tempid);
    void putUniqeID(const DWORD &tempid);
};

/**
 * \brief  回调基类
 */
class UserSkillExec
{
  public:
    virtual bool exec(zSkill *s) = 0;
};


/**
 * \brief  技能管理器基类
 */
class UserSkillM : private SkillManager
{
  public:
    UserSkillM();
    ~UserSkillM();
    zSkill *getSkillByTempID(DWORD id);
    void removeSkillByTempID(DWORD id);
    void removeSkill(zSkill *s);
    bool addSkill(zSkill *s);
    zSkill *findSkill(DWORD skillid);
    void execEvery(UserSkillExec &exec);
    void resetAllUseTime();
    void clearAllUseTime(DWORD skillID=0);
    void clear();
    DWORD getPointInTree(DWORD mySubkind);
    void refresh();
    int size() const;
    void clearskill(DWORD skillid);
};


//技能影响的状态数据
struct SkillState
{
  SkillState();
  void init();
  union
  {
    struct 
    {
      SWORD dvalue;    //伤害值增加固定数值1
      SWORD dvaluep;    //伤害值增加x%2
      SWORD pdefence;    //物理防御数值变更 57，86
      SWORD pdefencep;  //物理防御变更百分比
      SWORD updam;    //物理攻击增加固定数值
      SWORD updamp;    //物理攻击增加百分比
      SWORD umdam;    //法术攻击增加固定数值
      SWORD umdamp;    //法术攻击增加百分比
      SWORD dpdam;    //物理攻击减少固定数值
      SWORD dpdamp;    //物理攻击减少百分比
      SWORD dmdam;    //法术攻击减少固定数值
      SWORD dmdamp;    //法术攻击减少百分比
      SWORD updef;    //物理防御增加固定数值
      SWORD updefp;    //物理防御增加百分比
      SWORD umdef;    //法术防御增加固定数值
      SWORD umdefp;    //法术防御增加百分比
      SWORD dpdef;    //物理防御减少固定数值
      SWORD dpdefp;    //物理防御减少百分比
      SWORD dmdef;    //法术防御减少固定数值
      SWORD dmdefp;    //法术防御减少百分比
      SWORD topet;    //变为随机小动物79
      SWORD appenddam;  //产生额外伤害83
      SWORD movespeed;  //移动速度变更百分比 16，56
      SWORD mgspeed;    //减少技能施放间隔17
      SWORD coldp;    //减少陷入冰冻状态几率18
      SWORD poisonp;    //减少陷入中毒状态几率19
      SWORD petrifyp;    //减少陷入石化状态几率20
      SWORD blindp;    //减少陷入失明状态几率21
      SWORD chaosp;    //减少陷入混乱状态几率22
      SWORD atrating;    //命中率增加33,64
      SWORD reduce_atrating;  // 命中率减少 ZJW加入
      SWORD hpspeedup;  //生命值恢复速度增加34
      SWORD mpspeedup;  //法术值恢复速度增加35
      SWORD spspeedup;  //体力值恢复速度增加36
      SWORD akdodge;    //闪避率上升37
      SWORD reduce_akdodge;  // 闪避率下降
      SWORD mdefence;    //法术防御变更59，             // 40
      SWORD mdefencep;  //法术防御变更百分比x%
      SWORD uattackspeed;  //攻击速度提高白分比
      SWORD dattackspeed;  //攻击速度降低白分比
      SWORD sevendownp;  //降低陷入七大状态几率82
      SWORD tsfdamp;    //伤害转移百分比
      SWORD tsfdam;    //伤害转移数值
      SWORD passdam;    //被动：额外伤害117
      SWORD maxhp;       //生命值最大值变更
      SWORD maxmp;       //法术值最大值变更
      SWORD maxsp;       //体力值最大值变更
      SWORD pupdam;    //物理攻击增加固定数值（被动）
      SWORD pumdam;    //法术攻击增加固定数值（被动）
      SWORD pupdef;    //物理防御增加固定数值（被动）
      SWORD pumdef;    //法术防御增加固定数值（被动）
      SWORD supdam;    //弓箭类增加物理攻击力固定值(长效)
      SWORD spupdam;    //弓箭类增加物理攻击力固定值(被动)
      SWORD spupdamp;    //弓箭类增加物理攻击力百分比(被动)
      SWORD rpupdam;    //棍子类增加物理攻击力固定值(被动)
      SWORD lupdam;    //长效物理攻击力增强固定值
      SWORD satrating;  //弓箭类提升命中率
      SWORD patrating;   // 增加命中率（被动）
      SWORD pattackspeed;  //增加攻击速度（被动）
      SWORD upattribute;  //属性点增加
      SWORD tuling;      //土灵增强
      SWORD kulou;    //骷髅弓手增强
      SWORD tianbing;    //天兵增强
      SWORD weaponupdamp; //提升武器物理攻击力百分比
      SWORD weaponumdamp; //提升武器法术攻击力百分比
      SWORD uppetdamage; //提升召唤兽的攻击力
      SWORD uppetdefence; //提升召唤兽的防御力
      SWORD pmaxhp;    //被动提升生命最大值
      SWORD bang;      //增加人物施放出重击的几率
      SWORD theurgy_updam; //仙术增加物理攻击力
      SWORD theurgy_updamp; //仙术增加物理攻击力百分比
      SWORD theurgy_updef; //仙术增加物理防御力
      SWORD theurgy_umdefp; //仙术增加法术防御力百分比
      SWORD theurgy_umdef; //仙术增加法术防御力
      SWORD theurgy_umdam; //仙术增加法术攻击力
      SWORD theurgy_umdamp; //仙术增加法术攻击力百分比
      SWORD theurgy_updefp;//仙术增加物理防御力百分比
      SWORD theurgy_dpdef; //仙术降低物理防御力
      SWORD theurgy_dmdef; //仙术降低物理防御力
      SWORD theurgy_dpdam;    //物理攻击减少固定数值
      SWORD theurgy_dmdam;    //法术攻击减少固定数值
      SWORD hpupbylevel; //根据人物等级提升最大血量                    //80
      SWORD reflect_ardor; // 反弹灼热状态给攻击自己的敌人
      SWORD reflect_poison; //反弹中毒状态给攻击自己的敌人
      SWORD reflect_lull;  // 反弹麻痹状态给攻击自己的敌人
      SWORD reflect_frost; // 反弹冰冻状态给攻击自己的敌人
      WORD introject_maxmdam; // 召唤合体增加魔法攻击
      WORD introject_maxpdam; // 召唤合体增加物理攻击
      WORD introject_mdam; // 召唤合体增加魔法攻击
      WORD introject_pdam; // 召唤合体增加物理攻击
      WORD introject_mdef; // 召唤合体增加魔法防御
      WORD introject_pdef; // 召唤合体增加物理防御
      WORD introject_maxhp; // 召唤合体增加生命上限
      SWORD summonrelive; // 召唤兽重生
      SWORD pdamtodef;  // 物攻转物防
      SWORD mdamtodef;  // 魔攻转魔防
      SWORD pdeftodam;  // 物防转物攻
      SWORD mdeftodam;  // 魔防转魔攻
      SWORD sept_updamp;  // 提升物理攻击百分比
      SWORD sept_umdamp;  // 提升魔法攻击百分比
      SWORD sept_updefp;  // 提升物理防御百分比
      SWORD sept_umdefp;  // 提升法术防御百分比
      SWORD sept_maxhp;  // 提升生命最大值百分比
      SWORD sept_maxmp;  // 提升法术最大值百分比
      SWORD blazeappend; // 火焰系法术伤害提升比例千分之几
      SWORD pblazeappend; // 火焰系法术伤害提升比例千分之几
      SWORD levinappend; // 雷电系法术伤害提升比例千分之几
      SWORD plevinappend; // 雷电系法术伤害提升比例千分之几(被动)
      SWORD trapappend;  // 陷阱系法术伤害提升比例千分之几
      SWORD iceappend;  // 陷阱系法术伤害提升比例千分之几
      SWORD udef;      // 防御提升固定值（物理和法术都升）
      SWORD magicattack;  // 两仪心法，攻击的时候附带法术伤害
      SWORD array_udamp; // 阵法的双攻比率提升
      SWORD array_ddefp; // 阵法的双防比率降低
      SWORD array_dmvspeed; // 阵法的移动速度降低
      SWORD upcon; //提升体质
      SWORD upint; //提升智力
      SWORD updex; //提升敏捷
      SWORD sword_udam; //提升弓箭攻击的伤害点数   // 110
      SWORD attackaddhpnum; // 攻击增加自己生命值数目
      SWORD dodge;         // 有一定几率闪避敌人攻击
      SWORD reflect_icelull; // 反弹一个麻痹状态 冰雷盾用
      SWORD pumdamp;    //被动提升法术攻击力百分比
      SWORD addmenp;   // 增加精神百分比
      SWORD ice_umdefp; //冰甲用提升魔法防御百分比
      SWORD protectdam; //保护状态之伤害
      SWORD protectUpAtt; //保护状态之攻击翻倍
      SWORD unitarybattle; //一元阵法投送小技能到队长正在攻击的敌人身上，此为投送的技能ID
      SWORD teamappend; //组队攻击加成，变量里存放的是组队队长的ID
      SWORD nsc_updamp; //逆手刺的专用技能提升状态。
      SWORD tgyt_updefp; //铁骨御体专用技能提升状态
      SWORD tgzh_updef; //铁骨之魂专用技能提升状态
      SWORD lm_updamp;   // 鲁莽提升物理攻击力百分比
      SWORD upallattrib; // 提升所有属性数值
      SWORD dnallattrib; // 所有属性降低数值
      SWORD upstr;      // 提升力量属性百分比
      SWORD dpallattrib; // 所有属性下降百分比
      SWORD upattribstr; // 提升力量属性百分比
      SWORD upattribcon; // 提升体质属性百分比
      SWORD upattribmen; // 提升精神属性百分比
      SWORD upattribdex; // 提升敏捷属性百分比
      SWORD upattribint; // 提升智力属性百分比
      SWORD relive;    // 再生
      SWORD nowrelive; // 立即复活时回复百分比
      SWORD brappenddam; //猎手印记所提供的额外伤害清
      SWORD dpintmen;   // 智力精神降低百分比
      SWORD dpstrdex;   // 力量敏捷降低百分比
      SWORD enervation; // 衰弱
      SWORD erupt;      // 破釜沉舟状态 魔物攻击翻300%持续时间完后会获得复活虚弱状态

	  //sky 新增技能效果
	  SWORD AttAddDam;		//sky 增加伤害百分比
	  SWORD reflect;		//sky 反弹伤害固定值
	  SWORD reflectp;		//sky 反弹物理攻击伤害百分比
	  SWORD reflect2;		//sky 反弹魔法攻击伤害百分比
	  SWORD MagicReflex;	//sky 魔法系技能反射(直接把技能反射回去)
	  SWORD PhyReflex;		//sky 物理反射百分比
	  SWORD DaggerAddDam;	//sky 增加匕首伤害百分比
	  SWORD ThroAddDam;		//sky 增加投掷武器伤害百分比
	  SWORD HandsAddDam;	//sky 增加双手武器伤害百分比
	  SWORD HandAddDam;		//sky 增加单手武器伤害百分比
	  SWORD FlameAddDam;	//sky 增加火焰魔法伤害百分比
	  SWORD IceAddDam;		//sky 增加冰霜魔法伤害百分比
	  SWORD SacredAddDam;	//sky 增加神圣魔法伤害百分比
	  SWORD DarkAddDam;		//sky 增加黑暗魔法伤害百分比

	  SWORD DaggerAddDamA;	//sky 增加匕首伤害固定值
	  SWORD ThroAddDamA;	//sky 增加投掷武器伤害固定值
	  SWORD HandsAddDamA;	//sky 增加双手武器伤害固定值
	  SWORD HandAddDamA;	//sky 增加单手武器伤害固定值
	  SWORD FlameAddDamA;	//sky 增加火焰魔法伤害固定值
	  SWORD IceAddDamA;		//sky 增加冰霜魔法伤害固定值
	  SWORD SacredAddDamA;	//sky 增加神圣魔法伤害固定值
	  SWORD DarkAddDamA;	//sky 增加黑暗魔法伤害固定值
    };
    SWORD swdValue[171];
  };
};

//在每次换装备前调用
struct PkPreValue
{
  PkPreValue()
  {
    fiveexpress = 0;
    bzero(wdValue,sizeof(wdValue));
  }
  void init()
  {
    fiveexpress = 0;
    bzero(wdValue,sizeof(wdValue));
  }
  float fiveexpress;
  union
  {
    struct 
    {
      WORD fivedam;
      WORD fivemaxdam;
      WORD fivedef;
      WORD nofivedam;
      WORD nofivemaxdam;
      WORD nofivedef;
      WORD fivemdam;
      WORD fivemaxmdam;
      WORD fivemdef;
      WORD nofivemdam;
      WORD nofivemaxmdam;
      WORD nofivemdef;
    };
    WORD wdValue[12];
  };
};

struct PkValue
{
  PkValue()
  {
    bzero(dwValue,sizeof(dwValue));
  }
  void init()
  {
    bzero(dwValue,sizeof(dwValue));
  }
  union
  {
    struct 
    {
      DWORD  pdamage;          /// 物理攻击力 *
      DWORD  mdamage;          /// 法术攻击力 *
      DWORD  pdefence;          /// 物理防御力 *
      DWORD  mdefence;          /// 法术防御力 *
      DWORD  mcost;            /// 消耗法术值
      DWORD  hpcost;            /// 消耗生命值
      DWORD  spcost;            /// 消耗体力值
      DWORD  exp;            /// 经验消耗
      DWORD  dvalue;            /// 伤害值
      DWORD  dvaluep;          /// 伤害值增加百分比
      SWORD  damagebonus;        /// 技能伤害加成
    };
    DWORD dwValue[11];
  };
};

struct ScenePkState
{
  ScenePkState()
  {
    zRTime ctv;
    lastPTime = 0;
    lastMTime = 0;
    tGood = ctv;
    tGood.addDelay(ScenePkState::goodnessPeriod);
    tProtect = ctv;
    lastCheckGoodness = ctv;
    lastCheckGoodness.addDelay(ScenePkState::protectPeriod);
    protect_time=0;
  }


  //善恶度检查时间间隔
  static const int goodnessPeriod = 60 * 1000;
  zRTime tGood;

  //自卫时间间隔
  static const int protectPeriod = 10 * 1000;
  zRTime tProtect;

  //最后一次检查状态的时间
  zRTime lastCheckGoodness;

  //攻击人和自卫人记录
  struct attProtect
  {
    DWORD  id;
    BYTE  time;
    attProtect()
    {
      id = 0;
      time = 0;
    }
  };
  bool speedOutP(WORD speed,DWORD dwTime);
  bool speedOutM(WORD speed,DWORD dwTime);
  private:
  DWORD lastPTime;
  DWORD lastMTime;
  public:
  //好人杀死第一个坏人的时间
  zRTime tGoodNormal;
  void clearProtect()
  {
    protect_time=0; 
  }
  bool deathUserProtect(SceneUser *pThis,DWORD defid);
  bool cancelProtect(SceneUser *pThis,DWORD time=0);
  bool addProtect(SceneUser * pThis,DWORD time=0);
  bool hasProtected();
  DWORD leftProtectTime()
  {
    return protect_time;
  }
  private:
  DWORD protect_time;
};

struct zSkill;
using namespace SkillDef;
class ScenePk  
{
  public:
    static bool physicalMagicU2U(const Cmd::stAttackMagicUserCmd *rev,SceneUser *pAtt,SceneUser *pDef);
    static bool attackDeathUser(SceneUser *pAtt,SceneUser *pDef);
    static bool sendChangedUserData(SceneUser *pUser);
    static bool attackDeathNpc(SceneEntryPk *pAtt,SceneNpc *pDef);
    static bool attackFailToMe(const Cmd::stAttackMagicUserCmd *rev,SceneEntryPk * pAtt,bool failed=true,bool me=false);
    static void  checkProtect(SceneEntryPk *pAtt,SceneEntryPk *pDef);
    static bool attackUserCmdToNine(const Cmd::stAttackMagicUserCmd *rev,SceneEntryPk *pAtt);
    static void attackRTHpAndMp(SceneUser *pUser);
    static void attackRTExp(SceneUser *pUser,DWORD exp,DWORD dwTempID=0,BYTE byType=0);
    static bool attackRTCmdToNine(const Cmd::stAttackMagicUserCmd *rev,SceneEntryPk *pAtt,SceneEntryPk *pDef,const SDWORD sdwHP,BYTE byLuck);
    static bool checkAttackSpeed(SceneUser *pAtt,const Cmd::stAttackMagicUserCmd *rev);
    static void  calpdamU2U(const Cmd::stAttackMagicUserCmd *rev,SceneUser *pAtt,SceneUser *pDef);
    static void  calpdamU2N(const Cmd::stAttackMagicUserCmd *rev,SceneUser *pAtt,SceneNpc *pDef);
    static void  calmdamU2U(const Cmd::stAttackMagicUserCmd *rev,SceneUser *pAtt,SceneUser *pDef);
    static void calmdamU2N(const Cmd::stAttackMagicUserCmd *rev,SceneUser *pAtt,SceneNpc *pDef);
    static void calpdamN2U(const Cmd::stAttackMagicUserCmd *rev,SceneNpc *pAtt,SceneUser *pDef);
    static void calmdamN2U(const Cmd::stAttackMagicUserCmd *rev,SceneNpc *pAtt,SceneUser *pDef);
    static void calpdamN2N(const Cmd::stAttackMagicUserCmd *rev,SceneNpc *pAtt,SceneNpc *pDef);
    static void calmdamN2N(const Cmd::stAttackMagicUserCmd *rev,SceneNpc *pAtt,SceneNpc *pDef);
};

/// 技能善恶类型枚举
enum {
  SKILL_GOOD=0,
  SKILL_BAD=1
};

/// 技能状态类型枚举
enum {
  SKILL_TYPE_INITIAVITE=1,// 攻击技能
  SKILL_TYPE_RECOVERY=2,  // 临时被动技能
  SKILL_TYPE_PASSIVENESS=3  // 永久被动技能
};

/// 技能状态处理返回值枚举
enum {
  SKILL_ACTIVE  =  1,//  加到活动MAP中
  SKILL_RECOVERY,    //  加到临时被动MAP中
  SKILL_PASSIVENESS,  //  加到永久被动MAP中
  SKILL_RETURN,    //  不加到任何MAP中
  SKILL_BREAK,    //  不继续投放操作
};

/// 技能状态执行步骤枚举
enum {
  ACTION_STEP_DOPASS=  0,// 一个技能状态的开始步骤
  ACTION_STEP_START=  1,// 一个技能状态的开始步骤
  ACTION_STEP_TIMER,  // 一个技能状态的定时步骤
  ACTION_STEP_STOP,  // 一个技能状态的停止步骤
  ACTION_STEP_CLEAR,  // 一个技能状态被外部清除
  ACTION_STEP_RELOAD,    // 一个技能状态被重新载入
};

/// 技能状态最大数目定义
#define SKILLSTATENUMBER 354   /// 技能状态最大编号 [sky]扩充到400

/**
 * \brief  技能状态元素载体
 */
struct SkillStatusCarrier
{
    /// 技能操作
    const SkillStatus *status;
    /// 技能字典
    const zSkillB *skillbase;
    /// 收到的攻击消息
    Cmd::stAttackMagicUserCmd revCmd;
    /// 攻击者的指针
    SceneEntryPk *attacker;
    /**
     * \brief  构造函数，初始化所有属性
     */
    SkillStatusCarrier()
    {
      status = NULL;
      skillbase = NULL;
      attacker = NULL;
    }
};

/**
 * \brief  技能状态元素
 */
struct SkillStatusElement
{
  /// 状态的 id
  DWORD  id;

  /// 状态发生几率
  DWORD percent;

  /// 状态的影响数值(sky 召唤:怪物ID现在已经超过65535)
  DWORD value;

  /// 状态的持续时间
  QWORD qwTime;

  /// 状态执行的步骤标志
  DWORD state;


  ///攻击者的临时ID;
  DWORD dwTempID;

  ///攻击者的ID
  DWORD dwAttackerID;

  ///技能ID
  DWORD dwSkillID;

  ///执行时长
  DWORD dwTime;

  ///执行步骤
  BYTE  byStep;
  
  ///善恶类型
  BYTE  byGoodnessType;
  
  ///技能的互斥大类
  BYTE  byMutexType;

  ///是否刷新人物属性1为刷新0为否
  BYTE  refresh;

  ///攻击者的类型
  zSceneEntry::SceneEntryType attacktype;

  /**
   * \brief  技能状态元素构造函数初始化所有属性
   */
  SkillStatusElement()
  {
    id      = 0;          //状态的id;
    percent    = 0;
    value    = 0;
    qwTime    = 0;
    state    = 0;
    dwTempID  = 0;
    dwSkillID  = 0;
    dwTime    = 0;
    byStep    = 0;
    refresh    = 0;
  }
};

/**
 * \brief  技能状态管理器
 */
class SkillStatusManager
{
private:
  /// 主动技能状态列表
  std::map<DWORD,SkillStatusElement> _activeElement;

  /// 临时被动技能状态列表
  std::map<DWORD,SkillStatusElement> _recoveryElement;

  /// 永久被动技能状态列表
  std::map<DWORD,SkillStatusElement> _passivenessElement;

  /// 类型定义
  typedef std::map<DWORD,SkillStatusElement>::value_type VALUE_TYPE;

  /**
   * \brief  技能状态函数列表
   */
  struct{
    BYTE (* func)(SceneEntryPk *,SkillStatusElement &);
  }funlist[SKILLSTATENUMBER+1];

  /// 技能状态管理器属主
  SceneEntryPk *entry;
  bool bclearActiveSkillStatus;

public:
    SkillStatusManager();
    ~SkillStatusManager();
    void initMe(SceneEntryPk *pEntry);
    void loadSkillStatus(char *buf,DWORD length);
    void getSelectStates(Cmd::stSelectReturnStatesPropertyUserCmd *buf,unsigned long maxSize);
    void sendSelectStates(SceneEntryPk *pThis,DWORD state,WORD value,WORD time);
    void saveSkillStatus(char *buf,DWORD &size);
    bool putOperationToMe(const SkillStatusCarrier &carrier,const bool good = false,SWORD rangDamageBonus=0);
    void putPassivenessOperationToMe(const DWORD skillid,const SkillStatus *pSkillStatus);
    void processPassiveness();
    inline BYTE runStatusElement(SkillStatusElement &element);
    void timer();
    void clearActiveSkillStatus();
    void clearActiveSkillStatusOnlyUseToStatus48();
    void clearBadActiveSkillStatus();
    void addBadSkillStatusPersistTime(const DWORD &value);
    void addBadSkillStatusPersistTimePercent(const DWORD &value);
    void clearMapElement(const BYTE &byMutexType,std::map<DWORD,SkillStatusElement> &myMap,DWORD dwID,bool notify=true);
    WORD getSaveStatusSize();
    void clearRecoveryElement(DWORD value);
    void clearActiveElement(DWORD value);
    void processDeath();
    void clearSkill(DWORD dwSkillID);

//[test]-----------------------------------------------------------------
    void showValue();
    void showValueToLog();
    void showActive();
    void showRecovery();
    void showPassiveness();
//-----------------------------------------------------------------------

//sky 新增 begin
	void SetStatuToRecovery(DWORD id, DWORD time, DWORD percent, DWORD value=0, DWORD state=0);
//sky end
};

//sky 角色战斗状态枚举
enum useFightState
{
	USE_FIGHT_NULL	= 0,
	USE_FIGHT_PVE	= 1,
	USE_FIGHT_PVP	= 2,
};

/**
 * \brief 角色PK定义类,有待扩充
 */
struct SceneEntryPk: public zSceneEntry
{
  public:
    typedef std::set<DWORD> SelectedSet;
    typedef SelectedSet::value_type SelectedSet_value_type;
    typedef SelectedSet::iterator SelectedSet_iterator;
    ///被选中列表
    SelectedSet selected;
    //zMutex selected_lock;
  public:
    /**
     * \brief 构造函数,初始化
     *
     *
     * \param type 类型
     * \param state 初始状态
     * \return 
     */
    SceneEntryPk(SceneEntryType type,const SceneEntryState state = SceneEntry_Normal):zSceneEntry(type,state)
    {
      attackAction = true;
      moveAction = true;
      skillAction = true;
      ignoreDam = false;
      ignorePhysicDam = false; ///物理攻击免疫
      ignoreMagicDam = false;  ///魔法攻击免疫
      blind  = false;
      mplock = false;
      hplock = false;
      splock = false;
      maxattack=false;
      mdeftozero=false;   /// 魔法防御变成0
      pdeftozero=false;   /// 魔法防御变成0
      useRes=true;    // 可否使用物品
      isAutoRestitute=true;// 自动恢复HP,MP,SP
      frenzy = false; // 狂暴状态
      huntermark = false; //
      isPhysics =  false; // 走物理计算
      dread = false;// 恐吓
      dropweapon = false; //缴械
      tosh = false; //胡言乱语
      stone = false; // 石化
      watchTrap = false; //观察陷阱
      mhpd = false;
      swapdamcharid = 0; //  献身,施法者的临时ID
      swapdamcharid2 = 0; //关爱,伤害转移施法者的临时ID
      reSendData = false;
      changeface = false; // 易容术可以访问国外NPC
      hideme = false; //隐藏自己
      icebox = false; //冰灵柜状态
      liquidState = false; //液态（无阻挡）
      resist = 0;
      dmpbyhp = 0; //吸魔剑法
      mdmtomp = 0; //魔法伤害转换成魔法值
      mdmtompper = 0; //魔法伤害转换成魔法值几率
      icedam  = 0; //冰冻结界吸收的伤害值
      unitaryBattleKeepTime = 0;

      afreetMode = false; /// 恶魔杀无赦（砍人必死）
      angelMode = false;  /// 天使无敌（优于恶魔,砍不死）
      isPhysicBang = false;///是否暴击
      isMagicBang = false;///是否暴击
      isHPhysicBang = false;///是否暴击
      isHMagicBang = false;///是否暴击
      assault = false;
      curMagicManType =0;
      curMagicManID =0;
      lastPkTime=0;
      bombskillId = 0;
      dietodam = 0;
      attacklow = false; ///攻击力取下限
      killnow=0; ///一击必杀百分比
      diewithme=0; //血债血偿
      switchdie=0; //同屏内转移死亡状态
      summonsplit = 0; // 焰魔君主死亡后分裂标志
      damtohpper = 0;  /// 角色的伤害转化成生命值的几率;
      damtohpvalper = 0; /// 角色的伤害转化成生命值的百分比

	  Daggerflag = false;	//sky 匕首增加伤害标志
	  Throflag = false;		//sky 投掷武器增加伤害标志
	  Handsflag = false;	//sky 双手武器增加伤害标志
	  Handflag = false;		//sky 单手武器增加伤害标志
	  Flameflag = false;	//sky 火焰魔法增加伤害标志
	  Iceflag = false;		//sky 冰霜魔法增加伤害标志
	  Sacredflag = false;	//sky 神圣魔法增加伤害标志
	  Darkflag = false;		//sky 黑暗魔法增加伤害标志
	  Immolate = false;
	  dread = false;			/// 恐惧
	  blind = false;			/// 失明状态
	  frozen = false;			/// 冰冻
	  repentange = false;		/// 忏悔
	  Silence = false;			/// 沉默
	  Coma = false;				///	昏迷
	  Exile = false;			/// 放逐

	  ImmuneDread = false;		/// 免疫恐惧状态
	  ImmuneFrozen = false;		/// 免疫冰冻状态
	  ImmuneSilence = false;	/// 免疫沉默状态
	  ImmuneComa = false;		/// 免疫昏迷状态
	  ImmuneExile = false;		/// 免疫放逐状态 
	  Soulflag = false;

	  isUsePveOrPvp = USE_FIGHT_NULL; /// PVE状态

      reSendOther = false;

	  RandRegionX = 0;
	  RandRegionY = 0;

	  BattCampID = 0;
    }
    virtual ~SceneEntryPk(){};
    ///pk状态
    ScenePkState pkState;
    ///PK过程中变化的数值
    PkValue pkValue;
    ///技能附加的状态
    SkillState skillValue;
    ///PK过程相对不便的数值
    PkPreValue pkpreValue;

    SkillStatusManager skillStatusM;  /// 技能状态管理器
    SkillStatusCarrier carrier;      /// 技能操作投送器
    std::map<BYTE,BYTE>  _effectState;  /// 技能状态特效计数器
    SceneEntryPk *attackTarget;      /// 武器附带攻击状态攻击目标

    bool attackAction;  /// 攻击许可
    bool moveAction;  /// 移动许可
    bool skillAction;   /// 技能攻击许可
    bool ignoreDam;    /// 忽略伤害
    bool ignorePhysicDam; /// 忽略物理伤害
    bool ignoreMagicDam; /// 忽略魔法伤害
    bool notifyHMS;    /// 通知HP,MP,SP改变
    bool maxattack;    /// 攻击取上限
    bool mdeftozero;   /// 魔法防御变成0
    bool pdeftozero;   /// 魔法防御变成0
    bool useRes;    /// 可否使用物品
    bool isAutoRestitute; // 是否自动恢复
    bool frenzy;       /// 狂暴状态
    bool huntermark;  /// 猎手标记
    bool isPhysics;    /// 计算公式走物理   
    bool dropweapon;   /// 缴械
    bool tosh;        /// 胡言乱语
    bool stone;      /// 石化
    bool watchTrap;    /// 观察陷阱
    bool mhpd;  /// 魔法伤害减半,物理伤害加倍标志。
    bool blazeflag; ///火焰系法术加成标志
    bool levinflag;  ///雷电系法术加成标志
    bool trapflag;  ///陷阱系法术加成标志
    bool iceflag;  ///冰系法术加成标志
    DWORD swapdamcharid; ///献身施法者的临时ID
    DWORD swapdamcharid2; ///关爱施法者的临时ID
    bool changeface;  ///易容术,可以访问国外NPC
    bool hideme;     /// 隐藏自己
    bool icebox;   /// 冰灵柜状态
    bool liquidState; ///液态（无阻挡）
    BYTE resist;     ///格档,有一次抵御伤害的机会
    BYTE dmpbyhp;  ///吸魔剑法
    BYTE mdmtomp;  ///将法术伤害转成魔法值百分比
    BYTE mdmtompper; ///将法术伤害转成魔法值百分比几率
    SDWORD icedam;  ///冰冻结界吸收的伤害数值
    QWORD unitaryBattleKeepTime; //一元阵法持续时间
    SDWORD dietodam; ///死亡攻击数值
    bool attacklow; ///攻击力取下限
    BYTE killnow; //一击必杀
    BYTE diewithme; //血债血偿
    BYTE switchdie; //同屏转移死亡状态的几率
    BYTE summonsplit; //焰魔君主死亡后分裂标志


    bool reSendData;  /// 重新发送NPC或角色数据给自己
    bool reSendOther;  /// 重新发送NPC或角色数据给其它人

    bool mplock;  ///对mp进行锁定
    bool hplock;  ///对hp进行锁定
    bool splock;  ///对sp进行锁定

    bool afreetMode; /// 恶魔杀无赦（砍人必死）
    bool angelMode;  /// 天使无敌（优于恶魔,砍不死）

    bool isPhysicBang; /// 是否物理暴击
    bool isMagicBang; /// 是否魔法暴击
    bool isHPhysicBang; /// 是否物理暴击
    bool isHMagicBang; /// 是否魔法暴击

    bool assault; ///冲锋

	//sky 新增角色战斗状态 begin
	bool Daggerflag;	//sky 匕首增加伤害标志
	bool Throflag;		//sky 投掷武器增加伤害标志
	bool Handsflag;		//sky 双手武器增加伤害标志
	bool Handflag;		//sky 单手武器增加伤害标志
	bool Flameflag;		//sky 火焰魔法增加伤害标志
	bool Iceflag;		//sky 冰霜魔法增加伤害标志
	bool Sacredflag;	//sky 神圣魔法增加伤害标志
	bool Darkflag;		//sky 黑暗魔法增加伤害标志
	bool Immolate;		//sky 献祭标志

	bool dread;			/// sky 恐惧
	bool blind;			/// sky 失明状态
	bool frozen;		/// sky 冰冻
	bool repentange;	/// sky 忏悔
	bool Silence;		/// sky 沉默
	bool Coma;			///	sky 昏迷
	bool Exile;			/// sky 放逐

	bool ImmuneDread;	/// sky 免疫恐惧状态
	bool ImmuneFrozen;	/// sky 免疫冰冻状态
	bool ImmuneSilence;	/// sky 免疫沉默状态
	bool ImmuneComa;	/// sky 免疫昏迷状态
	bool ImmuneExile;	/// sky 免疫放逐状态
	//sky 灵魂状态,该状态下不可以见并且是无敌的
	bool Soulflag;

	//sky 新增角色状态(PVE:PVP)
	useFightState isUsePveOrPvp; /// PVE状态
	// sky end

	//sky 新增阵营ID
	//sky 用于战场中攻击和胜利判定用(重要)
	DWORD BattCampID;
	//sky end

    zPos keepPos;  /// 保存技能的攻击目标点,传递给需要使用的功能
    BYTE keepDir;  /// 保存技能的攻击方向,传递给需要使用的功能
    BYTE  lastPkTime; //最后PK标志
    DWORD bombskillId; /// 人物死亡后投送技能ID

    BYTE damtohpper;  /// 角色的伤害转化成生命值的几率;
    BYTE damtohpvalper; /// 角色的伤害转化成生命值的百分比

	//////////////////////////////////////////////////////////////////////////////////
	///sky 用来控制NPC被恐惧或者失明后移动的最大范围
	///随机范围的中心
	zPos RandPos;
	///随机最大范围的宽和高
	int RandRegionX,RandRegionY;

	///sky 判断是否超出随机范围
	virtual bool outOfRandRegion(zPos * pos) = 0;
	//////////////////////////////////////////////////////////////////////////////////

    ///所在的场景指针
    Scene *scene;

    //技能
    UserSkillM usm;

    virtual void sendtoSelectedHpAndMp() = 0;
    virtual void sendtoSelectedState(DWORD state,WORD value,WORD time) = 0;
    virtual void sendSevenStateToMe(DWORD state,WORD value,WORD time){}
    void sendCmdToSelected(const void *pstrCmd,const DWORD nCmdLen);

    /**
     * \brief 造成直接伤害
     * \param pAtt 攻击者
     * \param dam 伤害
     * \param notify 是否通知客户端
     * \return 伤害值
     */
    virtual SWORD directDamage(SceneEntryPk *pAtt,const SDWORD &dam,bool notify=false);

    /**
     * \brief 改变角色的hp
     * \param hp 变更的HP
     */
    virtual void changeHP(const SDWORD &hp)=0;

    /**
     * \brief 改变角色的sp
     * \param sp 变更的SP
     */
    virtual void changeSP(const SDWORD &sp)=0;

    /**
     * \brief 改变角色的mp
     * \param mp 变更的MP
     */
    virtual void changeMP(const SDWORD &mp)=0;

    /**
     * \brief 获得最大的hp
     * \return 返回最大值
     */
    virtual DWORD getMaxHP(){return 0;}

    /**
     * \brief 获得最大的hp
     * \return 返回最大值
     */
    virtual DWORD getBaseMaxHP(){return 0;}

    /**
     * \brief 获得最大的mp
     * \return 返回最大值
     */
    virtual DWORD getMaxMP(){return 0;}

    /**
     * \brief 获得最大的mp
     * \return 返回最大值
     */
    virtual DWORD getBaseMaxMP(){return 0;}

    /**
     * \brief 获得最大的sp
     * \return 返回最大值
     */
    virtual DWORD getMaxSP(){return 0;}

    /**
     * \brief 获得当前魔法攻击力
     * \return 魔法攻击力
     */
    virtual DWORD getMaxMDamage(){return 0;}

    /**
     * \brief 获得当前物理攻击力
     * \return 物理攻击力
     */
    virtual DWORD getMaxPDamage(){return 0;}

    /**
     * \brief 获得当前物理防御力
     * \return 物理防御力
     */
    virtual DWORD getPDefence(){return 0;}

    /**
     * \brief 获得当前魔法防御力
     * \return 魔法防御力
     */
    virtual DWORD getMDefence(){return 0;}

    /**
     * \brief 重置最大的hp
     */
    virtual void changeAndRefreshHMS(bool lock=true,bool sendData=true){}

    /**
     * \brief 在被自己攻击之前的处理,包括,装备损耗处理,攻击有效几率判断等
     * \param pUser 攻击者
     * \param rev 本次攻击的触发指令
     * \param physics 是否物理攻击
     * \param good 为true则必中,false要判断回避率
     * \return true为这次攻击是有效的,false为一次无效的攻击
     */
    virtual bool preAttackMe(SceneEntryPk *pUser,const Cmd::stAttackMagicUserCmd *rev,bool physics=true,const bool good = false)=0;

    /**
     * \brief 角色被攻击
     * \param pUser 攻击者
     * \param rev 本次攻击的触发消息
     * \param physics 是否物理攻击
     * \return true为这次攻击是有效的,false为一次无效的攻击
     */
    virtual bool AttackMe(SceneEntryPk *pUser,const Cmd::stAttackMagicUserCmd *rev,bool physics=true,SWORD rangDamageBonus=0);

    /**
     * \brief 设置角色身上的特效状态,对应状态的访问计数将累加
     * \param state 状态ID 根据enum SceneEntryState取值
     */
    //void setEffectStatus(const WORD &state)
    //{
      //_effectState[state]=_effectState[state]+1;
      //if (1 == _effectState[state])
      //{
        //showCurrentEffect(state,true);
      //}
    //}


    /**
     * \brief 清除角色身上的特效状态,对应状态的访问计数将减,减为0的时候清除该状态
     * \param state 状态ID 根据enum SceneEntryState取值
     */
    //void clearEffectStatus(const WORD &state)
    //{
      //_effectState[state]=_effectState[state]-1;
      //if (0 == _effectState[state])
      //{
        //showCurrentEffect(state,false);
      //}
    //}

    /**
     * \brief 让宠物重生
     */
    virtual void relivePet(){};

    /**
     * \brief 让角色重生
     */
    virtual void relive(){};

    /**
     * \brief 得到玩家的权限等级
     * \return 玩家的权限等级
     */
    virtual BYTE getPriv(){return 0;}

    /**
     * \brief 角色被击退N格
     */
    virtual void standBack(const DWORD dwAttTempID,DWORD grids)=0;

    /**
     * \brief 将攻击目标换成dwTempID所指向的角色玩家
     * \param dwTempID 目标角色的临时ID
     */
    virtual void changeAttackTarget(const DWORD &dwTempID){};

    /**
     * \brief 让角色死亡
     * \param dwTempID 攻击者的临时ID
     */
    virtual void toDie(const DWORD &dwTempID)=0;

    /**
     * \brief 在屏幕内随机移动
     */
    virtual void goToRandomScreen(){};

    /**
     * \brief 在屏幕内定点移动
     */
    virtual void jumpTo(zPos &newPos){};

    /**
     * \brief 通知客户端生命值的变化
     */
    virtual void attackRTHpAndMp(){};

    /**
     * \brief 判断角色是否死亡
     */
    virtual bool isDie() = 0;

    /**
     * \brief 获取角色的级别
     */
    virtual DWORD getLevel() const =0;

    /**
     * \brief 获取五行点数
     * \return 返回该类型的五行点数
     */
    virtual DWORD getFiveType() {return 0;}

    /**
     * \brief 需要的职业类型,决定可以使用的技能类型
     */
    virtual bool needType(const DWORD &needtype)=0;

    /**
     * \brief 需要的职业类型,决定可以使用的技能类型
     */
    virtual bool addSkillToMe(zSkill *skill)=0;

    /**
     * \brief 是否有该技能需要的武器
     * \return true 有 false 没有
     */
    virtual bool needWeapon(DWORD skillid)=0;

    /**
     * \brief 是否Pk区域
     * \param other PK相关人
     * \return true 是 false 否
     */
    virtual bool isPkZone(SceneEntryPk *other=NULL)=0;

    /**
     * \brief 依赖物品消耗型法术
     * \param object 消耗物品的类型
     * \param num 消耗物品的数量
     * \return true 消耗成功 false 失败
     */
    virtual bool reduce(const DWORD &object,const BYTE num)=0;

    /**
     * \brief 检查可消耗物品是否足够
     * \param object 消耗物品的类型
     * \param num 消耗物品的数量
     * \return true 足够 false 不够
     */
    virtual bool checkReduce(const DWORD &object,const BYTE num)=0;
    /**
     * \brief 获取装备伤害加成
     * \return 伤害加成
     */
    virtual WORD getDamageBonus() {return 0;}

    /**
     * \brief 施放技能所导致的消耗MP,HP,SP
     * \param base 技能基本属性对象
     * \return true 消耗成功 false 失败
     */
    virtual bool doSkillCost(const zSkillB *base) = 0;

    /**
     * \brief 检查是否骑马
     * \return true 骑马 false 没骑
     */
    virtual bool checkMountHorse() {return false;}

    /**
     * \brief 检查施放技能所导致的消耗MP,HP,SP是否足够
     * \param base 技能基本属性对象
     * \return true 消耗成功 false 失败
     */
    virtual bool checkSkillCost(const zSkillB *base) = 0;

    /**
     * \brief 检查自身的施放成功几率,决定这次技能是否可以施放
     * \return true 成功 false 失败
     */
    virtual bool checkPercent()=0;

    /**
     * \brief 判断是否是敌人
     * \return true 是 false 不是
     */
    virtual int isEnemy(SceneEntryPk *entry,bool notify = false,bool good = false)=0;

    /**
     * \brief 判断是否可攻击
     * \return true 是 false 不是
     */
    virtual bool isBugbear(){return true;}

    /**
     * \brief 获取自己的主人,一般针对NPC而言,Player的主人是自己
     * \return NULL或者主人的对象指针
     */
    virtual SceneEntryPk *getMaster()=0 ;

    /**
     * \brief 得到最上层的主人
     *
     * \return 主人
     */
    virtual SceneEntryPk *getTopMaster() =0;

    /**
     * \brief 检查飞行路径
     * \param pTarget 被攻击者
     * \param aType 飞行方式
     * \return 是否顺利攻击
     */
    virtual bool checkMagicFlyRoute(zSceneEntry *pTarget,BYTE aType);

    /**
     * \brief 角色死亡检查
     * \param pAtt 攻击者
     */
    virtual bool processDeath(SceneEntryPk *pAtt);
    virtual void leaveBattle();

    /**
     * \brief 通过杀死比自己等级高的npc来减少善恶度
     *
     * \param pNpc 杀死的npc
     */
    virtual void reduceGoodness(SceneNpc *pNpc){};

    virtual void setupCharBase(bool lock=true) {};
    /**
     * \brief 重新发送本角色的地图数据
     */
    virtual void reSendMyMapData()=0;

    /**
     * \brief 攻击者冲向我
     * \param attacktype 攻击者的类型
     * \param tempid 攻击者的临时id
     */
    virtual void assaultMe(BYTE attacktype,DWORD tempid) {}

    /**
     * \brief 召唤宠物
     */
    //virtual ScenePet* summonPet(DWORD id,Cmd::petType type) {return NULL;}
    /**
     * \brief 召唤图腾
     */
	virtual ScenePet*	summonPet(DWORD id,Cmd::petType type,DWORD standTime = 0,DWORD sid=0,const char * petName = "",DWORD anpcid=0,zPos pos=zPos(0,0),BYTE dir=4) = 0;

	//[sky]幻影专用召唤函数
	virtual	bool	MirageSummonPet(DWORD id,Cmd::petType type,DWORD standTime,WORD num,const char * petName = "",DWORD anpcid = 0,zPos pos=zPos(0,0),BYTE dir=4) = 0;
    //召唤指定持续时间的宠物

	//[sky] 召唤士兵(每个士兵都是一个独立的个体不同以召唤兽或者宠物)
	virtual int summonSoldiers(DWORD id, Cmd::petType type, WORD num, DWORD sid=0, const char * name="", DWORD anpcid = 0,zPos pos=zPos(0,0),BYTE dir=4) = 0;

    /**
     * \brief 设置战斗类宠物的跟踪攻击目标
     */
    virtual void setPetsChaseTarget(SceneEntryPk *entry){};

    /**
     * \brief 设置战斗类宠物的跟踪攻击目标
     * \param weaponType 武器类型
     * \return true武器类型符合,false武器类型不符合
     */
    virtual bool checkWeapon(BYTE weaponType){return true;}


    /**
     * \brief 获取抗毒增加  
     */
    virtual SWORD getPoisondef(){return 0;}
    /**
     * \brief 获取抗麻痹增加  
     */
    virtual SWORD getLulldef(){return 0;}
    /**
     * \brief 获取抗眩晕增加  
     */
    virtual SWORD getReeldef(){return 0;}
    /**
     * \brief 获取抗噬魔增加  
     */
    virtual SWORD getEvildef(){return 0;}
    /**
     * \brief 获取抗噬力增加  
     */
    virtual SWORD getBitedef(){return 0;}
    /**
     * \brief 获取抗混乱增加  
     */
    virtual SWORD getChaosdef(){return 0;}
    /**
     * \brief 获取抗冰冻增加  
     */
    virtual SWORD getColddef(){return 0;}
    /**
     * \brief 获取抗石化增加  
     */
    virtual SWORD getPetrifydef(){return 0;}
    /**
     * \brief 获取抗失明增加  
     */
    virtual SWORD getBlinddef(){return 0;}
    /**
     * \brief 获取抗定身增加  
     */
    virtual SWORD getStabledef(){return 0;}
    /**
     * \brief 获取抗减速增加  
     */
    virtual SWORD getSlowdef(){return 0;}
    /**
     * \brief 获取抗诱惑增加
     */
    virtual SWORD getLuredef(){return 0;}


    /**
     * \brief 获取毒增加  
     */
    virtual SWORD getPoison(){return 0;}
    /**
     * \brief 获取麻痹增加  
     */
    virtual SWORD getLull(){return 0;}
    /**
     * \brief 获取眩晕增加  
     */
    virtual SWORD getReel(){return 0;}
    /**
     * \brief 获取噬魔增加  
     */
    virtual SWORD getEvil(){return 0;}
    /**
     * \brief 获取噬力增加  
     */
    virtual SWORD getBite(){return 0;}
    /**
     * \brief 获取混乱增加  
     */
    virtual SWORD getChaos(){return 0;}
    /**
     * \brief 获取冰冻增加  
     */
    virtual SWORD getCold(){return 0;}
    /**
     * \brief 获取石化增加  
     */
    virtual SWORD getPetrify(){return 0;}
    /**
     * \brief 获取失明增加  
     */
    virtual SWORD getBlind(){return 0;}
    /**
     * \brief 获取定身增加  
     */
    virtual SWORD getStable(){return 0;}
    /**
     * \brief 获取减速增加  
     */
    virtual SWORD getSlow(){return 0;}
    /**
     * \brief 获取诱惑增加
     */
    virtual SWORD getLure(){return 0;}


    virtual void appendAttack(const Cmd::stAttackMagicUserCmd *rev) {};


    ///宠物
    ScenePet * pet;
    ///召唤兽
    ScenePet * summon;
    ///图腾
    std::list<ScenePet *> totems;

	///[sky]幻影专用多NPC召唤
	std::list<ScenePet *> MirageSummon;

    /**
     * \brief 取消召唤的宠物
     */
    virtual bool killOnePet(ScenePet *)=0;
    virtual void killAllPets()=0;
    virtual void killSummon() {};

    virtual void sendPetDataToNine();
    virtual void addPetExp(DWORD,bool,bool) = 0;
    void petLevelUp(ScenePet *);

    virtual bool isRedNamed(bool allRedMode=true) const = 0;
    DWORD getHp();
    DWORD getMaxHp();
    bool isFighting();
    ///脱离战斗状态的时间
    zRTime endBattleTime;
    ///施法者
    DWORD curMagicManID;
    ///对施法者类型
    DWORD curMagicManType;
    ///对手
    DWORD curTargetID;
    ///对手类型
    DWORD curTargetType;
    virtual bool setCurTarget(SceneEntryPk *,bool=false);
    virtual bool setCurTarget(DWORD,DWORD,bool=false);
    SceneEntryPk * getCurTarget();

    ///防御的对手
    DWORD defTargetID;
    ///防御的对手类型
    DWORD defTargetType;
    virtual bool setDefTarget(SceneEntryPk *,bool=false);
    virtual bool setDefTarget(DWORD,DWORD,bool=false);
    SceneEntryPk * getDefTarget();
    void clearDefTarget();


    void setEndBattleTime(const zRTime &,int);
    bool checkEndBattleTime(const zRTime &);

    virtual WORD skillUpLevel(WORD skilltype,WORD skillkind){return 0;}

    virtual void getSummonAppendDamage(WORD &minDamage,WORD &maxDamage){};
    virtual void exchangeMeAndSummonPet(){};
    virtual DWORD getMasterMana(){return 0;}
    virtual DWORD getMP(){return 0;}
    virtual void clearMana(){}
    virtual void leech(DWORD dwDam) {}
    virtual void sendMessageToMe(const char *pattern) {};
    virtual void setSpeedRate(float rate){};
    virtual void resetSpeedRate(){};
    /**
     * \brief 设置角色的当前状态,并根据当前状态呈现角色的特效或者关闭特效
     * \param state 状态ID 根据enum SceneEntryState取值
     * \param isShow 是否显示特效
     */
    virtual void showCurrentEffect(const WORD &state,bool isShow,bool notify=true) = 0;
  private:
    virtual void processMaskOnAttack(SceneEntryPk *){};
    virtual void processMaskOnDefence(){};
    virtual void processAddDam(int &dwDam,int &dwDamDef,bool physics){};
    virtual void reduceDam(int &dwDam,int &dwDamDef,bool physics){};
    virtual void reflectDam(int &dwDamDef,int &dwDamSelf,DWORD skillID){};
    virtual void hp2mp(int &dwDamDef){};
    virtual void reflectSkill(SceneEntryPk *pAtt,const Cmd::stAttackMagicUserCmd *rev){};
};

#define MAX_NPC_GROUP 5

/**
 * \brief 场景物件的回调类
 *
 * 只要是用于遍历一屏物件,而对某些物件操作
 *
 * 使用者必须要实现exec方法
 */
struct zSceneEntryCallBack
{
  virtual bool exec(zSceneEntry *entry)=0;
  virtual ~zSceneEntryCallBack(){};
};


/**
 * \brief 伤害转移回调函数,随机将伤害转移到一屏内的某一个可伤害对象
  */
struct DamagCallback: public zSceneEntryCallBack
{
  zSceneEntry *me;
  SceneEntryPk *target;
  DamagCallback(SceneEntryPk *entry)
  {
    me = entry;
    target = NULL;
  }
  bool exec(zSceneEntry *entry)
  {
    zSceneEntry::SceneEntryType myType = entry->getType();
    if (entry == me ) return false;
    if (myType == zSceneEntry::SceneEntry_NPC)
    {
      if (((SceneEntryPk *)entry)->isBugbear())
      {
        target = (SceneEntryPk *)entry;
        if (selectByPercent(40)) return false;
      }
      else
      {
        return true;
      }
    }
    else if (myType == zSceneEntry::SceneEntry_Player)
    {
      target = (SceneEntryPk *)entry;
      if (selectByPercent(40)) return false;
    }
    return true;
  }
  SWORD action(SDWORD dam)
  {
    if (target)
    {
      return target->directDamage((SceneEntryPk *)me,dam);
    }
    return 0;
  }
};

typedef std::vector<SceneEntryPk *> SceneEntryPk_vec;

enum LeechdomType  //sky 老的物品效果类型
{
  Leechdom_default,
  Leechdom_dam,//增加伤害力
  Leechdom_def,//增加防御力
  Leechdom_poison,//药物使人持续中毒
  Leechdom_sppersist,//物品使人体力值保持当前值不变
  Leechdom_spup,//药物增加人体力值百分比
  Leechdom_spcostdown,//减慢体力消耗速度
  Leechdom_spresumeup,//加快体力恢复速度
  Leechdom_hp,//一次性恢复生命
  Leechdom_hppersist,//持续恢复生命
  Leechdom_mp,//一次性恢复法术值
  Leechdom_mppersist,//持续恢复法术
  Leechdom_sp,//一次性恢复体力值
  Leechdom_hp5,//一次性恢复生命
  Leechdom_hppersist5,//持续恢复生命
  Leechdom_hpmax,//消耗耐久一次加满HP
  Leechdom_mpmax,//消耗耐久一次加满MP
  Leechdom_chocolate,//巧克力
};

struct SceneUser;
struct LeechdomElement
{
  LeechdomElement()
  {
    type = Leechdom_default;
    value = 0;
    times = 0;
  }
  LeechdomType type;
  WORD value;
  WORD times;
};
/**
 * \brief 药品使用
 *
 */
struct Leechdom
{
  Leechdom()
  {
    damcooling = 0;
    defcooling = 0;
    hpcooling = 0;
    spcooling = 0;
    mpcooling = 0;
    sppersist = 0;
  }
  void add(LeechdomType type,WORD value,WORD times);
  void clear();
  DWORD fresh(SceneUser *pUser,DWORD &update);
  bool isCooling(DWORD type);
  void checkCooling();
  private:
  typedef std::vector<LeechdomElement> Leechdom_vector;
  typedef Leechdom_vector::iterator Leechdom_iterator;
  Leechdom_vector element;
  zMutex mlock;
  /// 伤害类物品冷却时间
  WORD damcooling;
  /// 防御类物品冷却时间
  WORD defcooling;
  /// 恢复hp类物品冷却时间
  WORD hpcooling;
  /// 恢复sp类物品冷却时间
  WORD spcooling;
  /// 恢复mp类物品冷却时间
  WORD mpcooling;
  WORD sppersist;
};

class LiveSkillsManager;

class LiveSkills
{
public:
  LiveSkills();
  ~LiveSkills();
    

  std::size_t count()
  {
    return skills.size();
  }
  
  void add(const LiveSkill& skill)
  {
    skills[skill.id] = skill;  
  }
  
  enum {
    START_SKILL_ID = 321,
  };
  
  int state() const
  {
    /*
    int state = skill_id - LiveSkills::START_SKILL_ID + Cmd::LIVE_SKILL_STATE_START + 1;

    //the following case should not be occured,just for safe
    state = (state<0)?0:state;
    state = (state>Cmd::MAX_STATE)?Cmd::MAX_STATE:state;
    */
    int state = 0;
    /*switch (skill_id)
    {
      case 321:
        state = Cmd::USTATE_PLANTING;
        break;
      case 324:
        state = Cmd::USTATE_DIGGING;
        break;
      case 327:
        state = Cmd::USTATE_PROSPECTING;
        break;
      case 330:
        state = Cmd::USTATE_CUTTING;
        break;
      case 333:
        state = Cmd::USTATE_GATHERING;
        break;
      case 336:
        state = Cmd::USTATE_FISHING;
        break;
      case 337:
        state = Cmd::USTATE_HUNTING;
        break;
    }   */   
    
    return state;
  }
  
  //notice,unsafe function,ensure length of buf greater than 6
  int titles(BYTE* buf,int len) const
  {
    assert( len>=6 );
    memset(buf,0,len);
    
    const_iterator it = skills.begin();
    while (it != skills.end()) {
      switch (it->second.id) 
      {
        case 323: //缝纫
          buf[0] = (BYTE) it->second.level;
          break;          
        case 326: //铁匠
          buf[1] = (BYTE) it->second.level;
          break;          
        case 329: //首饰
          buf[2] = (BYTE) it->second.level;
          break;          
        case 331: //木匠
          buf[3] = (BYTE) it->second.level;
          break;          
        case 335: //炼药
          buf[4] = (BYTE) it->second.level;
          break;          
        case 338: //烹饪
          buf[5] = (BYTE) it->second.level;
          break;          
      }      
      ++it;  
    }
    
    return 0;
  }
  
  bool is_upgrade() const
  {
    bool result = true;
    
    switch (skill_id)
    {
      case 333:
        result = false;
        break;
      case 336:
        result = false;
        break;
      case 337:
        result = false;
        break;
    }
    
    return result;
  }
  
private:
  friend class LiveSkillsManager;
  friend class LiveSkill;
  friend class SceneUser;
  friend class EquipMaker;
  typedef std::map<WORD,LiveSkill> skill_t;
  typedef skill_t::iterator iterator;
  typedef skill_t::const_iterator const_iterator;
  
  skill_t skills; //用户当前获得技能
  unsigned long working_time; //用户上次工作时间
  WORD skill_id; //用户上次工作类别
  zObject* tool; //劳动工具
  /**
   * \brief 依据技能id查找用户是否具有该技能
   * \return 找到的技能,没有返回NULL
   */
  LiveSkill* skill(WORD id);
  
};

class zObject;

class LiveSkillsManager
{
public:

  /**
   * \brief 生活技能相关的消息处理函数
   * 
   */
  //static int execute(SceneUser* user,const Cmd::t_NullCmd *command,DWORD len);

  /**
   * \brief 劳动状态指令过滤
   * 
   */
  static bool command_filter(SceneUser* user,const Cmd::t_NullCmd *pNullCmd,const DWORD cmdLen);


  /**
   * \brief 开始劳动处理函数
   * 
   */  
  int execute(SceneUser* user,zObject* ob);

  /**
   * \brief 更新用户劳动状态
   * 
   */
  static bool update(SceneUser* user);
  
  /**
   * \brief 读入某用户的生活技能信息
   * 
   */
  static int load(SceneUser* user,const LiveSkill& live_skill);

  /**
   * \brief 按照特定格式存储用户生活技能信息
   * \param user:要存档的用户
   * \param dest:目标地址
   * \return 封包后的缓冲区长度
   */
  static int save(SceneUser* user,BYTE* dest);  
  
  /**
   * \brief 通知客户端某用户的生活技能信息,在SceneUser::sendInitToMe被调用
   * 
   */
  static void notify(SceneUser* user);

  /**
   * \brief 实现sigleton模式
   * 
   */  
  static LiveSkillsManager& instance();

private:
  static LiveSkillsManager* instance_;
  
  LiveSkillsManager();
  ~LiveSkillsManager();

  class Factory : public zDatabaseCallBack<zLiveSkillB> 
  {
  public:  
    typedef hash_set<DWORD> hash_set;
    typedef hash_set::iterator iterator;

    hash_set identifies; //all possiable work tools
    std::map<DWORD,DWORD> skills; //
    
    bool exec(zLiveSkillB* entry)
    {
      //identifies.insert((entry->map_kind << 16) | (entry->weapon_kind ));
      if (entry->weapon_kind) {
        identifies.insert( entry->weapon_kind );
        skills[(entry->map_kind<<16) | entry->weapon_kind] = entry->skill_id;
      }
      return true;
    }
    
  };
  
  Factory factory;
};

//马的类型
const DWORD HORSE_TYPE_NOTHORSE = 0;//没马
const DWORD HORSE_TYPE_NORMAL = 1;//普通
const DWORD HORSE_TYPE_BATTLE = 2;//战马
const DWORD HORSE_TYPE_SUPER = 3;//高级战马

//马的PK数据.骑乘时加到主人身上的
struct t_HorsePkData
{
    DWORD id;       //npcid
    char name[MAX_NAMESIZE];//名字
    DWORD lv;       //等级
    WORD str;       //力量
    WORD intel;     //智力
    WORD dex;       //敏捷
    WORD men;       //精神
    WORD con;       //体质
    WORD speed;     //速度
    BYTE pdam;      //增加物理攻击力%
    BYTE pdef;      //增加物理防御力%
    BYTE mdam;      //增加魔法攻击力%
    BYTE mdef;      //增加魔法防御力%
    WORD maxhp;     //最大HP
    WORD maxmp;     //最大MP
    DWORD callTime;     //召唤间隔
    Cmd::horseState state;   //马的状态
    WORD poisonRes;     //抗毒
    WORD lullRes;       //抗麻痹
    WORD faintRes;      //抗晕眩
    WORD chaosRes;      //抗混乱
    WORD freezeRes;     //抗冰冻
    WORD petrifyRes;    //抗石化
    WORD blindRes;      //抗失明
    WORD slowRes;       //抗减速
    DWORD horseid;      //马的id（道具id）
};

/**
 * \brief 马匹类
 *
 * 封装了有关马匹的处理函数
 */
class Horse
{
public:
  Horse(SceneUser& user);  
  
  ~Horse();

  void horse(DWORD horse_id);
  bool horse(t_Object &);
  
  DWORD horse() const;

  void mount(bool flag, WORD speed=0, bool send=true);
  
  bool mount() const;
  
  int load(BYTE* );
  
  int save(BYTE*);

  void setSummonTime(int delay);
  bool checkSummonTime();
  bool isFollowing();
  bool comeOut();
  bool putAway();

  bool full_HorseDataStruct(Cmd::t_HorseData *);

  void sendData();

  bool canFight();

  Cmd::t_HorseData data;
  t_HorsePkData pkData;
  void getPkData();

  DWORD horseType();
  DWORD horseType(DWORD);
private:
  DWORD interval;
  zRTime summonTime;
  SceneUser& _owner;
  //Cmd::horseState _state;
  DWORD _horse;  
  bool _mount;

  DWORD dwID;
  //bool _follow;
};

/**
 * \brief 任务
 *
 * 该类封装了对任务的抽象
 *
 */
class Quest
{
public:
  enum {
    FINISHED_SAVE = -1,
    FINISHED_NOT_SAVE = -2,
    FINISHED = -3,
    DOING = 0,
  };
  
  static bool execute(SceneUser& user,Cmd::stQuestUserCmd* cmd,DWORD len);
  
  static int load(SceneUser& user,BYTE* dest,unsigned long &dest_size);
  
  static int save(SceneUser& user,BYTE* dest);
  
  static int notify(SceneUser& user);
  
  static int abandon(SceneUser& user,DWORD id);

  /**     
   * \brief  返回任务名称
   *
   * \return 任务名称
   */  
  const std::string& title() const
  {
    return _title;
  }
  
  /**     
   * \brief  返回任务描述
   *
   * \return 任务描述
   */  
  const std::string& description() const  
  {
    return _description;
  }

  static const std::string FINISHED_NAME;
    
private:
  friend class QuestTable;
    
  DWORD _id;
  
  std::string _title;
  std::string _description;
};

/**
 * \brief 任务变量
 *
 * 该类封装了对任务变量的抽象
 *
 */
class Vars
{
public:
  typedef   void (*callback_func) ();

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param quest_id: 任务id
   */      
  Vars(DWORD quest_id = 0) : _quest_id(quest_id),_timeout(0),_start_time(0),_update(0)
  { }
  
  /**     
   * \brief 析构函数
   *
   */    
  ~Vars()
  { }

  void callback(callback_func func)
  {
    _func = func;
  }

  template<typename T>
  bool get_value(const std::string& name,T& value)
  {
    var_iterator it = _vars.find(name);
    if (it != _vars.end()) {
      std::stringstream os(it->second.value());
      os >> value;
      return true;
    }

    return false;
  }
  /**     
   * \brief  设置变量
   *
   * 根据Operation类型设置变量值
   *      
   * \param op: 操作类型
   * \param name: 变量名称
   * \param action: 要改变的变量值
   * \param tmp: 变量是否需要存储
   * \return 当前总是返回0
   */  
  template <typename Operation>
  int set_value(Operation op,const std::string& name,typename Operation::value_type const & action,int tmp=0,SceneUser *user=NULL)
  {
    std::stringstream os;
    
    var_iterator it = _vars.find(name);
    if (it != _vars.end()) {
      os << it->second.value();
      tmp = it->second.tmp();

    }else {
      //haven't this var before,give a default value  
      os << 0;
    }
        
    typename Operation::value_type value;
    os >> value;

    //value=action;
    op(value,action,user);

    os.clear();
    
    os << value;
    std::string new_value;
    os >> new_value;

    _vars[name] = VAR(new_value,tmp);
    
    return 0;
  }

  /**     
   * \brief  判断变量是否有效
   *
   * 根据Operation类型判断变量值是否满足条件要求
   *      
   * \param op: 操作类型
   * \param name: 变量名称
   * \param condition: 测试值
   * \return 有效返回true,否则返回false
   */    
  template <typename Operation>
  bool is_valid(Operation op,const std::string& name,typename Operation::value_type const & condition) const
  {
    const_var_iterator it = _vars.find(name);
    if (it != _vars.end()) {
      std::stringstream os(it->second.value());
      
      typename Operation::value_type value;
      os >> value;
            
      return op(value,condition);
    }
    

    return !Operation::NEED_EXIST;      
  }
  
  /**     
   * \brief  返回任务id
   *
   * \return 任务id
   */  
  DWORD quest_id() const
  {
    return _quest_id;
  }
  
  int set_timer();
  int set_timer(int start);
  
  bool is_timeout(int timeout) const;
  int start_time() const;
  
  int save_timer(BYTE* dest) const;
  int load_timer(BYTE* dest);

  int save(BYTE* dest) const;
  int load(BYTE* dest);
  
  int notify(SceneUser& user) const;
  int notify(SceneUser& user,const std::string& name) const;
  
  int state() const;

  int update() const;
  void update(int value);

  std::string info() const;

  bool reserve(); 
private:
  class VAR
  {
  public:
    VAR(const std::string& value="",int tmp=0) : _value(value),_tmp(tmp)
    { }
    
    ~VAR()
    { }
    
    const std::string& value() const
    {
      return _value;
    }
    
    bool is_tmp() const
    {
      return _tmp==1;  
    }
    
    bool tmp() const
    {
      return _tmp;
    }
    
  private:
    std::string _value;
    int _tmp;  
  };  
  /*struct key_hash : public std::unary_function<const std::string,size_t>
  {
    size_t operator()(const std::string &x) const
    {
      hash<const char *> H;
      return H(x.c_str());
    }
  };*/
  typedef hash_map<std::string,VAR> VARS;
  typedef VARS::iterator var_iterator;
  typedef VARS::const_iterator const_var_iterator;  
    
  VARS _vars;  
  DWORD _quest_id;
  int _timeout;
  int _start_time;
  int _update;

  callback_func _func;
};

/**
 * \brief 全局变量
 *
 * 该类封装了对全局任务变量的抽象
 *
 */

template <int dummy>
class GlobalVarImpl
{
public:
  typedef GlobalVarImpl<dummy> self_t;
  enum {
    MAX_BUF_SIZE = 4096*8,
  };
  
  static self_t& instance()
  {
    if (!_instance) {
      /***********************
      static self_t new_instance; 
      _instance = &new_instance;
      ************************/
      //here can not use as above
      _instance = new self_t;
    }
    
    return *_instance;
  }    

  Vars* vars(QWORD id) const
  {
    const_vars_iterator it = _vars.find(id);
    if   (it != _vars.end() ) {
      return it->second;
    }
    
    return NULL;  
  }

  Vars* add(QWORD id)
  {
    Vars* v = vars(id);

    if (!v) {
      v = new Vars(id);
      _vars[id] = v;
    }

    return v;
  }

  void update()
  {
    time_t t = time(NULL);
    const struct tm * const now = localtime(&t);

    if (_day != now->tm_mday) {
      _day = now->tm_mday;
      destroy();
    }

  }

  bool save() const
  {
    std::ofstream of(_file.c_str(),std::ios::binary);

    char buf[MAX_BUF_SIZE*30];
    bzero(buf,sizeof(buf));

    int count = _vars.size();
    memccpy(buf,&count,sizeof(count),sizeof(buf));
    int len = sizeof(count);
    
    for (const_vars_iterator it=_vars.begin(); it!=_vars.end(); ++it) {
      QWORD id = it->first;
      memccpy(buf+len,&id,sizeof(id),sizeof(buf) - len);
      len += sizeof(id);

      len += it->second->save((BYTE*)buf+len);
      if (len >= (MAX_BUF_SIZE*30 - 1024))
      {
        Zebra::logger->fatal("存储全局变量时,缓冲区过短越界（%u,%u）",MAX_BUF_SIZE,len);
        of.write(buf,len);
        bzero(buf,sizeof(buf));
        len = 0;
      }
    }

    if (len > 0) of.write(buf,len);
      
    of.flush();
    of.close();
    
    return true;
  }

  bool load()
  {
    bool ret = false;
    std::ifstream inf(_file.c_str(),std::ios::binary);

    //获得文件大小
    inf.seekg(0,std::ios::end);
    int length = inf.tellg();
    inf.seekg(0,std::ios::beg);

    if (length > 0)
    {
      char *buf = new char[length];
      if (buf)
      {
        bzero(buf,length);

        inf.read(buf,length);
        int size =*((int*)buf);
        int len = sizeof(int);

        while (size-- > 0) {
          QWORD id = *((QWORD*)(buf+len));
          len += sizeof(id);
          Vars* vars = new Vars;
          len += vars->load((BYTE*)buf+len);
          _vars[id] = vars;
        }
        SAFE_DELETE_VEC(buf);
        ret = true;
      }
      else
        Zebra::logger->fatal("加载任务变量文件分配内存失败：%u",length);
    }

    inf.close();
    return ret;
  }

/*
  void set(QWORD id,const std::string& name,int value)
  {
    Vars* v = vars(id);

    if (!v) {
      v = new Vars(id);
      _vars[id] = v;
    }

    Op::Set<int> op;
    v->set_value(op,name,value);
  }
*/

  std::string show() const
  {
    std::ostringstream os;
    for (const_vars_iterator it=_vars.begin(); it!=_vars.end(); ++it) {
      os << "变量(" << it->first << ")" << "\n";
      os << it->second->info() << "\n";
    }
    return os.str();
  }

  static void server_id(int id) 
  {
    SERVER_ID = id;
  }

  static int server_id()
  {
    return SERVER_ID;
  }
  
private:  
  void destroy()
  {  
/*
    for (vars_iterator it=_vars.begin(); it!=_vars.end(); ++it) {
      SAFE_DELETE(it->second);
    }
    _vars.clear();
*/
    for (vars_iterator it=_vars.begin(); it!=_vars.end(); ) {
      if (it->second->reserve()) ++it;
      else {
        vars_iterator tmp = it;
        ++it;
        SAFE_DELETE(tmp->second);
        _vars.erase(tmp);
      }
    }
  }
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   */ 
  GlobalVarImpl()
  { 
    time_t t = time(NULL);
    const struct tm * const now = localtime(&t);
    _day = now->tm_mday;

    std::ostringstream os;
//    os << ScenesService::getInstance().getServerID();
    os << Zebra::global["vardir"];
    os << SERVER_ID << "_";
    os << dummy << ".sv";
    _file = os.str();
  }
  
  /**     
   * \brief 析构函数
   *
   */    
  ~GlobalVarImpl()
  { 
    destroy();
  }
  
  typedef std::map<QWORD,Vars*> VARS;  //
  typedef VARS::iterator vars_iterator;
  typedef VARS::const_iterator const_vars_iterator;
  
  VARS _vars;
  
  static self_t* _instance;
  int _day;
  std::string _file;

  static int SERVER_ID;
};

template <int dummy>
GlobalVarImpl<dummy>* GlobalVarImpl<dummy>::_instance = NULL;

template <int dummy>
int GlobalVarImpl<dummy>::SERVER_ID = 0;

typedef GlobalVarImpl<1> GlobalVar;
typedef GlobalVarImpl<2> TongVar;
typedef GlobalVarImpl<3> FamilyVar;
typedef GlobalVarImpl<4> UsersVar;

#define ALLVARS(function) \
  GlobalVar::instance().function(); \
  TongVar::instance().function(); \
  FamilyVar::instance().function(); \
  UsersVar::instance().function(); \
  UserVar::instance().function();

/*************************************************************
NOTE: This macro used initial the filename,according by server id,
           it must be call FIRST,even BEFORE the INSTANCE.
*************************************************************/
#define ALLVARS1(function,arg1) \
  GlobalVar::function(arg1); \
  TongVar::function(arg1); \
  FamilyVar::function(arg1); \
  UsersVar::function(arg1); \
  UserVar::function(arg1);


/**
 * \brief 用户变量
 *
 * 该类封装了对用户任务变量的抽象
 *
 */
class UserVar
{
public:
  enum {
    MAX_BUF_SIZE = 4096*8,
  };
  static UserVar& instance();

  Vars* vars(DWORD id,QWORD key) const;
  
  Vars* add(DWORD id,QWORD key);

  void update()
  {
//    time_t t = time(NULL);
//    const struct tm * const now = localtime(&t);
//
//    if (_day != now->tm_mday) {
//      _day = now->tm_mday;
//      destroy();
//    }

  }

  bool save() const;
  bool load();

/*
  void set(QWORD id,const std::string& name,int value)
  {
    Vars* v = vars(id);

    if (!v) {
      v = new Vars(id);
      _vars[id] = v;
    }

    Op::Set<int> op;
    v->set_value(op,name,value);
  }
*/

  std::string show() const
  {
    std::ostringstream os;
    for (const_vars_iterator it=_vars.begin(); it!=_vars.end(); ++it) {
      os << "变量(" << it->first << ")" << "\n";
      //os << it->second->info() << "\n";
    }
    return os.str();
  }

  static void server_id(int id) 
  {
    SERVER_ID = id;
  }

  static int server_id()
  {
    return SERVER_ID;
  }
private:  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   */ 
  UserVar()
  {
    std::ostringstream os;
//    os << ScenesService::getInstance().getServerID();
    os << Zebra::global["vardir"];
    os << SERVER_ID << "_";
    os << "userv" << ".sv";
    _file = os.str();
  }
  
  /**     
   * \brief 析构函数
   *
   */    
  ~UserVar()
  { }
  
  class VAR
  {
  public:  
    Vars* vars(QWORD key) const
    {
      const_var_iterator it = _vars.find(key);
      if   (it != _vars.end() ) {
        return it->second;
      }
      
      return NULL;
    }    
    
    Vars* add(DWORD id,QWORD key)
    {
      Vars* v = vars(key);
      if (!v) v = new Vars(id);
    
      _vars[key] = v;
    
      return v;        
    }
    
    /**     
     * \brief 存储变量
     *
     *存储任务变量
     *      
     * \param dest: 任务档案
     * \return 存储的二进制档案长度
     */
    int save(BYTE* dest) const;

    /**     
     * \brief 读取变量
     *
     *读取任务变量
     *      
     * \param dest: 任务档案
     * \return 读取的二进制档案长度
     */
    int load(BYTE* dest);
  private:
    std::map<QWORD,Vars*> _vars;
    typedef std::map<QWORD,Vars*>::const_iterator const_var_iterator;
  };
  
  typedef hash_map<DWORD,VAR*> VARS;  //
  typedef VARS::iterator vars_iterator;
  typedef VARS::const_iterator const_vars_iterator;
  std::string _file;
  
  VARS _vars;
  
  static UserVar* _instance;
  static int SERVER_ID;
};

/**
 * \brief 任务列表
 *
 * 该类封装了对用户身上的任务列表的操作
 *
 */
class QuestList
{
public:
  enum {
    MAX_NUM = 20,
  };
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   */ 
  QuestList() : _menu("")
  { }

  /**     
   * \brief 析构函数
   *
   */    
  ~QuestList()
  { }
  
  Vars* vars(DWORD id) const;
  
  void add_quest(DWORD id,const Vars& vars,SceneUser& user,bool notify = true);
  int count() const;
  
  bool set_menu(const std::string& menu);
  int get_menu(char* menu,int& status);
  
  void add_menu(const std::string& menu);
  
  int save(BYTE* dest) const;
  int load(BYTE* dest,unsigned long &dest_size);
  
  int state(DWORD id) const;
  int start_time(DWORD id) const;
  
  int notify(SceneUser& user) const;
  
  int abandon(SceneUser& user,DWORD id,bool force = false,bool destroy = true);

  // 清除所有任务
  void clear(SceneUser* pUser);

  int update(SceneUser& user,bool refresh);

  std::string info(int id=0) const;

private:
  
  std::string _menu;
  std::list<std::string> _subs;
  
  typedef hash_map<DWORD,Vars> QUESTS;  //questid,vars
  typedef QUESTS::iterator quest_iterator;
  typedef QUESTS::const_iterator const_quest_iterator;
  
  QUESTS _quests;

};

template <typename T>
class EventManager;

/**
 * \brief 事件驱动表格
 *
 *管理不同类型的事件列表,并提供统一的处理方式
 *
 */  
class EventTable
{
public:
  
  static EventTable& instance();

  /**     
   * \brief  执行任务事件
   *
   * 调用相应的事件管理器执行该任务事件
   *      
   * \param user: 请求的用户
   * \param event:任务事件
   * \return 处理结果
   */    
  template <typename E>
  int execute (SceneUser& user,E& event)
  {
    return EventManager<E>::instance().execute(user,event);
  }

  /**     
   * \brief  执行任务事件
   *
   * 调用相应的事件管理器执行该任务事件
   *      
   * \param event:任务事件
   * \return 处理结果
   */    
  template <typename E>
  int execute (E& event)
  {
    return EventManager<E>::instance().execute(event);
  }

  bool init(const std::string& type,const std::string& file);
  
private:  

  EventTable();
  ~EventTable();
  
  static EventTable* _instance;
};

/**
 * \brief 任务表格
 *
 *任务集合,存储各个任务的详细信息
 *
 */  
class QuestTable
{
public:
  static QuestTable& instance();
  
  bool init();
  
  const Quest* quest(DWORD id) const;
  
private:
  QuestTable();

  ~QuestTable();

  static QuestTable* _instance;  
  
  typedef hash_map<DWORD,Quest> QUESTS;
  typedef QUESTS::iterator quest_iterator;
  typedef QUESTS::const_iterator const_quest_iterator;
  
  QUESTS _quests;
};

#define magicrange_hash(type,dir) ((type * 2) + dir)
#define MAX_RELATIVE_POS 20
/**
 * \brief 相对坐标数据结构
 *
 */
struct RelativePos : public zPos
{
  DWORD w; /// 伤害损耗
  RelativePos()
  {
  }
  zPos getAbsolutePos(const zPos &center,DWORD dir);
};
typedef std::vector<RelativePos> MagicRange;
typedef struct {
  MagicRange lib;
  DWORD num;
} SMagicRange;
/**
 * \brief 技能格字回调
 *
 */
struct MagicPosExec
{
  virtual ~MagicPosExec(){};
  virtual bool exec(zPos &pos) = 0;
};
/**
 * \brief 技能作用点
 *
 */
struct MagicPos
{
  MagicPos(const DWORD type,const DWORD dir);
  void execEvery(MagicPosExec &pos);
  private:
    SMagicRange range;
};

/**
 * \brief 初始化技能群攻类型范围
 *
 */
class MagicRangeInit
{

  public:

    ~MagicRangeInit()
    {
      final();
    }

    static MagicRangeInit &getInstance()
    {
      if (NULL == instance)
        instance = new MagicRangeInit();

      return *instance;
    }

    static void delInstance()
    {
      SAFE_DELETE(instance);
    }

    bool init();
    void get(const DWORD type,const DWORD dir,SMagicRange &range);
    void final();

  private:

    static MagicRangeInit *instance;

    MagicRangeInit() {};

    typedef hash_map<DWORD,SMagicRange> MagicRangeContainer;
    zRWLock rwlock;
    MagicRangeContainer ranges;

};

class CountryDare
{
  public:
    CountryDare()
    {
      last_attack_time=0;
      id=0;last_gen_attack_time=0;
      last_king_attack_time=0;
    };
    
    ~CountryDare(){};
    
    DWORD id;
    time_t last_attack_time;
    time_t last_gen_attack_time;
    time_t last_att_gen_attack_time;
    time_t last_king_attack_time;
};

class CountryDareM
{
  public:
    static CountryDareM& getMe()
    {
      if (!_instance)
      {
        _instance = new CountryDareM();
      }

      return *_instance;
    }
    
    ~CountryDareM(){}

    /**
      * \brief 时间回调函数
      */
    void timer();


    /**
     * \brief 判断是否可以攻击国旗
     *
     */
    bool isAttackMainFlag(Scene* scene,DWORD dwNpcID);

    /**
     * \brief 判断是否可以攻击大将军
     *
     */
    bool isAttackMainGen(Scene* scene);

    
    /**
      * \brief 复活副旗
      *
      */
    void reliveSecondFlag(Scene* scene,DWORD dwNpcID);

    /**
      * \brief 延长副旗复活时间
      *
      */
    void delaySecondFlag(Scene* scene);

    /**
     * \brief 更新骚扰国战被攻击时间
     *
     */
    void updateAttackTime(DWORD countryid,time_t uptime);
    void updateKingAttackTime(DWORD countryid,time_t uptime);
    void setCountryDare(DWORD dwCountryID,DWORD dwDareCountryID,bool isAtt);

    /**
     * \brief 
     *
     */
    void updateGenAttackTime(DWORD countryid,time_t uptime);

    void updateAttGenAttackTime(DWORD countryid,time_t uptime);

    /**
     * \brief 初始化
     *
     * 初始化该场景所有国家的国战信息
     *
     */
    void init();
    
  protected:
    std::vector<CountryDare> dares;
    zRWLock rwlock;  
    CountryDareM(){}
    static CountryDareM* _instance;
};

class CTech
{
  public:
    CTech()
    {
      dwType = 0;
      dwLevel = 0;
    }
    
    ~CTech(){};
    void init(Cmd::Session::_techItem* rec);
    bool canProduce(DWORD dwObjectLevel);

    DWORD dwType;
    DWORD dwLevel;
};

class CountryTech
{
  public:
    CountryTech(){};
    ~CountryTech(){};
    typedef std::map<DWORD,CTech*> CTechMap;

    void init(Cmd::Session::t_updateTech_SceneSession* rev);
    /// 科技索引
    CTechMap techIndex;
    CTech* getTech(DWORD dwType);
    void   addTech(DWORD dwType,CTech* pTech);
    DWORD id;

    zRWLock rwlock;
};

class CountryTechM : public Singleton<CountryTechM>
{
  friend class SingletonFactory<CountryTechM>;
  public:
    ~CountryTechM(){}

    /**
     * \brief 初始化
     *
     * 初始化该场景所有国家的国家科技信息
     *
     */
    void init();
    CountryTech* getCountryTech(DWORD dwCountryID);
    void   addCountryTech(DWORD dwCountryID);
    bool canProduce(DWORD dwCountry,DWORD dwObjectType,DWORD dwObjectLevel);
    
  protected:
    typedef std::map<DWORD,CountryTech*> CountryTechMap;
    /// 科技索引
    CountryTechMap countryTechIndex;
    CountryTechM(){}
    std::map<DWORD,DWORD> TechMap;
    zRWLock rwlock;
};

class CAlly
{
  public:
    CAlly()
    {
      dwCountryID = 0;
      dwAllyCountryID = 0;
      dwFriendDegree = 0;
    }

    ~CAlly(){};

    DWORD dwCountryID;
    DWORD dwAllyCountryID;
    DWORD dwFriendDegree;
};

class CountryAllyM : public Singleton<CountryAllyM>
{
  friend class SingletonFactory<CountryAllyM>;
  public:
  ~CountryAllyM(){}

  /**
   * \brief 初始化
   *
   * 初始化该场景所有国家的国家同盟信息
   *
   */
  void init();
  CAlly* getAlly(DWORD dwCountryID1,DWORD dwCountryID2);
  void   processUpdate(Cmd::Session::t_updateAlly_SceneSession *rev);
  bool   isAlly(DWORD dwCountryID1,DWORD dwCountryID2);
  DWORD  getFriendLevel(DWORD dwCountryID1,DWORD dwCountryID2);

  protected:
  std::vector<CAlly*> allies;
  CountryAllyM(){}

  zRWLock rwlock;
};

/**
 * \brief 触发事件
 *
 * 封装了对任务脚本触发的事件的处理
 *
 */
class Event
{
public:
  enum {
    START_QUEST = 0,
    QUESTING = 1,
  };
  
  Event();
  ~Event();


  int execute();
  int execute(SceneUser& user);
  int execute(SceneUser& user,DWORD quest_id,DWORD offset);
  
  
  int state(SceneUser& user);

  /**     
   * \brief   取得事件id
   *      
   * \return 事件id
   */
  DWORD id() const
  {
    return _id;
  }
  
  bool parse(const std::string& file_name);

  const char * npc_name() const
  {
    return _npc_name;
  }
private:
  
  class FreeMemory 
  {
  public:
    template <typename T>
    void operator () (T target)
    {
      delete target;  
    }  
  };

  class Embranchment
  {
  public:
    Embranchment();

    ~Embranchment();

    bool is_valid(SceneUser* user,const Vars* vars);
    int do_it(SceneUser* user,Vars* vars);

    void add(Condition* condition);
    void add(Action* action);
    bool active() const;
    void active(bool flag);
  private:
    bool _active;
    std::vector<Condition *> _conditions;
    std::vector<Action *> _actions;
  };
  
  typedef hash_map<DWORD,Embranchment *> EMBRANCHMENT; //embranchment_id
  typedef EMBRANCHMENT::iterator embranchment_iterator;
  typedef EMBRANCHMENT::const_iterator const_embranchment_iterator;  
  
  //EMBRANCHMENT _embranchments;
  
  //we use the higest bit to indicate if it is a new quest
  template <typename T>
  class LessIgnoreHB : public std::binary_function<T,T,bool>
  {
  public:
    bool operator() (T preceding,T posterior) const
    {
      preceding &= ~(0x1 << 31);
      posterior &= ~(0x1 << 31);
      
      return preceding < posterior;
    }
  };
  
  typedef std::map<DWORD,EMBRANCHMENT *,LessIgnoreHB<DWORD> > QUESTS; //quest_id
  typedef QUESTS::iterator quest_iterator;
  
  QUESTS _quests;
  
  DWORD _id;
  char _npc_name[MAX_NAMESIZE];
};

/**
 * \brief 事件触发器
 *
 *触发事件模板,提供对不同触发事件类型的统一处理
 *
 */
template<typename T>
class EventTrigger
{
public:
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   */    
  EventTrigger(DWORD id) : _id(id)
  { }

  /**     
   * \brief   取得事件id
   *      
   * \return 事件id
   */    
  DWORD id() const
  {
    return _id;
  }
  
  /**     
   * \brief   取得事件目录
   *      
   * \return 事件目录
   */
  static const std::string& dir()
  {
    return T::DIR;
  }
  
private:
  DWORD _id;    
};

namespace Trigger {
  /**
   * \brief 使用触发事件
   *
   *在使用物品时被触发
   *
   */  
  class Use
  {
  public:
    static const std::string DIR;
  };
  
  /**
   * \brief 杀死触发事件
   *
   *在杀死NPC时被触发
   *
   */  
  class Kill
  {
  public:
    static const std::string DIR;
  };

  /**
   * \brief 杀死触发事件
   *
   *在杀死NPC时被触发
   *
   */  
  class KillByLevel
  {
  public:
    static const std::string DIR;
  };
  
  /**
   * \brief 杀死触发事件(亲自杀的)
   *
   *在杀死NPC时被触发
   *
   */  
  class KillBySelf
  {
  public:
    static const std::string DIR;
  };

  /**
   * \brief 拾取触发事件
   *
   *在拾取物品时被触发
   *
   */  
  class Get
  {
  public:  
    static const std::string DIR;
  };
    
  /**
   * \brief 访问触发事件
   *
   *在访问NPC时被触发
   *
   */  
  class Visit
  {
  public:  
    static const std::string DIR;
  };

  /**
   * \brief 死亡触发事件
   *
   *在用户死亡时被触发
   *
   */  
  class Die
  {
  public:  
    static const std::string DIR;
  };

  /**
   * \brief 骑马触发事件
   *
   *在用户骑马时被触发
   *
   */  
  class Ride
  {
  public:  
    static const std::string DIR;
  };

  /**
   * \brief 退出触发事件
   *
   *在用户退出时被触发
   *
   */  
  class Quit
  {
  public:  
    static const std::string DIR;
  };

  /**
   * \brief 定时触发事件
   *
   *需要定时处理的任务
   *
   */  
  class Timer
  {
  public:  
    static const std::string DIR;
  };
  
  /**
   * \brief 丢弃触发事件
   *
   *在用户丢弃物品时被触发
   *
   */  
  class Drop
  {
  public:  
    static const std::string DIR;
  };

  /**
   * \brief 进入游戏触发事件
   *
   *在用户进入游戏时被触发
   *
   */  
  class Enter
  {
  public:  
    static const std::string DIR;
  };

  /**
   * \brief 其他触发事件
   *
   */  
  class Other
  {
  public:  
    static const std::string DIR;
  };  
}


//typedef EventTrigger<Trigger::Use> OnUse;
typedef EventTrigger<Trigger::Kill> OnKill;
typedef EventTrigger<Trigger::KillByLevel> OnKillByLevel;
typedef EventTrigger<Trigger::KillBySelf> OnKillBySelf;
typedef EventTrigger<Trigger::Get> OnGet;
//typedef EventTrigger<Trigger::Visit> OnVisit;
typedef EventTrigger<Trigger::Die> OnDie;
typedef EventTrigger<Trigger::Ride> OnRide;
typedef EventTrigger<Trigger::Quit> OnQuit;
typedef EventTrigger<Trigger::Timer> OnTimer;
typedef EventTrigger<Trigger::Drop> OnDrop;
typedef EventTrigger<Trigger::Enter> OnEnter;
typedef EventTrigger<Trigger::Other> OnOther;

/**
 * \brief 访问触发事件
 *
 *在访问NPC时被触发
 *
 */  
template <typename T>
class EventTrigger2 : public EventTrigger<T> 
{
public:  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *  
   * param id : 访问的npc id
   * param quest_id : 任务id
   * param offset : 任务分支 
   */    
  EventTrigger2(DWORD id,DWORD quest_id = 0,DWORD offset = 0) : EventTrigger<T>(id),_quest_id(quest_id),_offset(offset)
  { }

  /**     
   * \brief   取得事件对应的任务id
   *      
   * \return 任务id
   */  
  DWORD quest_id() const
  {
    return _quest_id;
  }
  
  /**     
   * \brief   取得事件对应的任务分支
   *      
   * \return 任务分支
   */  
  DWORD offset() const
  {
    return _offset;
  }
    
private:
  DWORD _quest_id;
  DWORD _offset;
};

typedef EventTrigger2<Trigger::Use> OnUse;
typedef EventTrigger2<Trigger::Visit> OnVisit;

/**
 * \brief 事件管理器
 *
 *管理不同类型的事件列表,并提供统一的处理方式
 *
 */  
template <typename E>
class EventManager
{
public:
  typedef EventManager<E> self_t;

  /**     
   * \brief  单件模式,保证事件管理器的唯一实例
   *      
   * \return 事件管理器的唯一实例
   */    
  static self_t& instance() 
  {
    static self_t new_instance;      
    return new_instance;
  }

  /**     
   * \brief  执行任务
   *
   * 查找对应的任务事件并执行
   *      
   * \param user: 请求的用户
   * \param event:任务事件
   * \return 处理结果
   */  
  int execute(SceneUser& user,E& event)
  {
    hash_map<DWORD,Event>::iterator it = _events.find(event.id());
    if (it!=_events.end()) {
      return it->second.execute(user);
    }
    
    //return Action::SUCCESS;
    return 0;
  }

  /**     
   * \brief  执行任务
   *
   * 查找对应的任务事件并执行
   *      
   * \param event:任务事件
   * \return 处理结果
   */    
  int execute(E& event);

  /**     
   * \brief  任务状态
   *
   * 取得用户某任务的状态
   *      
   * \param user: 请求的用户
   * \param event: 查询的任务
   * \return 任务状态
   */  
  int state(SceneUser& user,E& event);

  /**     
   * \brief  解析任务脚本
   *
   * 解析任务脚本,生成对应的事件结构
   *      
   * \param file_name: 文件名
   * \return true表示解析脚本成功,false表示解析脚本失败
   */    
  bool parse(const std::string& file_name) 
  {
    Event event;
    
    std::string name = Zebra::global["questdir"]+E::dir()+file_name;
    if (!event.parse(name) ) {
      return false;
    }
    
    _events[event.id()] = event;
    return true;
  }
  
  void get_valid_quest(SceneUser &user,bool=false);
  void get_valid_quest_str(SceneUser &user);
private:
  hash_map<DWORD,Event> _events;  

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *  
   */    
  EventManager()
  { }

  /**     
   * \brief 析构函数
   *
   */
  ~EventManager()
  { }
  
  static EventManager<E>* _instance;
};

template<typename E>
EventManager<E>* EventManager<E>::_instance = NULL;

/**     
 * \brief  执行任务
 *
 * 模板偏特化,执行特定的访问类型触发任务事件
 *      
 * \param user: 请求的用户
 * \param event:任务事件
 * \return 处理结果
 */  
template<>
inline int EventManager<OnVisit>::execute(SceneUser& user,OnVisit& event)
{
  hash_map<DWORD,Event>::iterator it = _events.find(event.id());
  if (it!=_events.end()) {
    if (event.quest_id() && event.offset() ) {
      return it->second.execute(user,event.quest_id(),event.offset());
    }else {
      return it->second.execute(user);  
    }
  }
  
  return 0;
}

template<>
inline int EventManager<OnUse>::execute(SceneUser& user,OnUse& event)
{
  hash_map<DWORD,Event>::iterator it = _events.find(event.id());
  if (it!=_events.end()) {
    if (event.quest_id() && event.offset() ) {
      return it->second.execute(user,event.quest_id(),event.offset());
    }else {
      return it->second.execute(user);  
    }
  }
  
  return 0;
}

/**     
 * \brief  任务状态
 *
 * 取得用户某任务的状态
 *      
 * \param user: 请求的用户
 * \param event: 查询的任务
 * \return 任务状态
 */  
template<>
inline int EventManager<OnVisit>::state(SceneUser& user,OnVisit& event)
{
  hash_map<DWORD,Event>::iterator it = _events.find(event.id());
  if (it!=_events.end()) {
    return it->second.state(user);  
  }
  
  return -1;
}

/**     
 * \brief  执行任务
 *
 * 模板偏特化,执行特定的定时触发任务事件
 *      
 * \param event:任务事件
 * \return 处理结果
 */  
template<>
inline int EventManager<OnTimer>::execute(OnTimer& event)
{
  hash_map<DWORD,Event>::iterator it = _events.find(event.id());
  if (it!=_events.end()) {
    return it->second.execute();  
  }
  
  return 0;
}

//配置文件中关于区域类型定义
/**
 * \brief 区域类型的定义
 * 包括类型、大小等
 *
 */
struct ZoneTypeDef
{
  enum
  {
    ZONE_NONE        =  0, //一般区
    ZONE_PK_SAFE      =  1,   // pk安全区
    ZONE_ABSOLUTE_SAFE    =  2,   // 绝对安全区
    ZONE_RELIVE        =  4,   // 死亡重生区
    ZONE_NEWBIE        =  8,   // 新手出生区
    ZONE_SPORTS        =  16,   // 竞技区
    ZONE_FOREIGN_RELIVE    =  32,   // 国外死亡重生区
    ZONE_PRIVATE_STORE     =   128,      // 摆摊区
    ZONE_PRIVATE_DARE     =   256,      // 国战跳转区
    ZONE_PRIVATE_RELIVE    =  512,    // 国战复活区
    ZONE_PRIVATE_UNION    =   1024,     // 帮会所属地跳转区
    ZONE_COUNTRY_WAR    =   2048,    // 边境的国战跳转区
    ZONE_PRIVATE_DARE_UNION    =  4096,    // 帮会战挑战方跳转区
    ZONE_DARE_SAFE      =   8192,    // 国战时为绝对安全区,平时为非安全区
    ZONE_PRIVATE_THIRD_UNION        =       16384,    // 帮会所属地第三方跳转区
    ZONE_EMPEROR_DEF_RELIVE    =   32768,    // 皇城战守方复活区
    ZONE_EMPEROR_ATT_RELIVE    =  65536,    // 皇城战攻方复活区  
  };

  zPos  pos;                //坐标
  WORD  width;              //矩形宽
  WORD  height;              //矩形长
  DWORD type;                //矩形区域的类型
  zSceneEntry::SceneEntryState initstate;  //初始状态,隐藏或者非隐藏
  zSceneEntry::SceneEntryState state;    //当前状态,隐藏或者非隐藏
  zRegion region;              //范围数据,包括范围内的可行走点数据索引

  /**
   * \brief 构造函数
   *
   */
  ZoneTypeDef() : pos(),region()
  {
    width = 0;
    height = 0;
    type = 0;
    initstate = zSceneEntry::SceneEntry_Normal;
    state = zSceneEntry::SceneEntry_Normal;
  }
  /**
   * \brief 拷贝构造函数
   *
   */
  ZoneTypeDef(const ZoneTypeDef &zone)
  {
    pos = zone.pos;
    width = zone.width;
    height = zone.height;
    type = zone.type;
    initstate = zone.initstate;
    state = zone.state;
    region = zone.region;
  }
  /**
   * \brief 赋值操作符号
   *
   */
  ZoneTypeDef & operator= (const ZoneTypeDef &zone)
  {
    pos = zone.pos;
    width = zone.width;
    height = zone.height;
    type = zone.type;
    initstate = zone.initstate;
    state = zone.state;
    region = zone.region;
    return *this;
  }
};

typedef std::vector<ZoneTypeDef> ZoneTypeDefVector;

const DWORD NANJIAO_MAP_ID = 135;
const DWORD DONGJIAO_MAP_ID = 136;
const DWORD BIANJING_MAP_ID = 137;
const DWORD WANGCHENG_MAP_ID = 139;
const DWORD PK_MAP_ID = 213;

//配置文件中关于NPC的定义
/**
 * \brief npc定义结构
 * 包括编号,出生位置、范围、间隔等信息 
 *
 */
struct t_NpcDefine
{
	DWORD id;                //编号
	char  name[MAX_NAMESIZE+1];        //名称
	zPos  pos;                //坐标
	WORD  width;              //矩形宽
	WORD  height;              //矩形长
	DWORD num;                //数量
	DWORD interval;              //刷新时间
	zSceneEntry::SceneEntryState initstate;  //初始状态,隐藏或者非隐藏
	zRegion region;              //范围数据,包括范围内的可行走点数据索引

	//std::vector<zPos> path;//固定移动的路线

	DWORD rushID;//触发攻城ID
	BYTE rushRate;//触发攻城的几率
	DWORD rushDelay;//触发攻城的延时

	int scriptID;//智能脚本的id
	DWORD intervalAmendment;		//sky 复活时间修正值
	DWORD AmendmentFactor;		//sky 修正系数
	WORD Camp;					//sky 所属阵营索引值

	std::map<DWORD,std::pair<DWORD,DWORD> > petList;
	std::map<DWORD,std::pair<DWORD,DWORD> > summonList;
	std::map<DWORD,std::pair<DWORD,DWORD> > deathSummonList;
	std::list< std::pair<DWORD,zPos> > dieList;
  /**
   * \brief 构造函数
   *
   */
  t_NpcDefine() : pos(),region()
  {
    id = 0;
    bzero(name,sizeof(name));
    width = 0;
    height = 0;
    num = 0;
    interval = 0;
    rushID = 0;
    rushRate = 0;
    rushDelay = 0;
    initstate = zSceneEntry::SceneEntry_Normal;
    scriptID = 0;
	intervalAmendment = 0;
	AmendmentFactor = 0;
	Camp = 0;
  }
  /**
   * \brief 拷贝构造函数
   *
   */
  t_NpcDefine(const t_NpcDefine &reg)
  {
    id = reg.id;
    bcopy((void *)reg.name,name,sizeof(name),sizeof(name));
    pos = reg.pos;
    width = reg.width;
    height = reg.height;
    num = reg.num;
    interval = reg.interval;
    initstate = reg.initstate;
    region = reg.region;
    //path = reg.path;
    rushID = reg.rushID;
    rushRate = reg.rushRate;
    rushDelay = reg.rushDelay;
    scriptID = reg.scriptID;
    petList = reg.petList;
    summonList = reg.summonList;
    deathSummonList = reg.deathSummonList;
    dieList = reg.dieList;
	intervalAmendment = reg.intervalAmendment;
	AmendmentFactor = reg.AmendmentFactor;
	Camp = reg.Camp;
  }
  /**
   * \brief 赋值操作符号
   *
   */
  t_NpcDefine & operator= (const t_NpcDefine &reg)
  {
    id = reg.id;
    bcopy((void *)reg.name,name,sizeof(name),sizeof(name));
    pos = reg.pos;
    width = reg.width;
    height = reg.height;
    num = reg.num;
    interval = reg.interval;
    initstate = reg.initstate;
    region = reg.region;
    //path = reg.path;
    rushID = reg.rushID;
    rushRate = reg.rushRate;
    rushDelay = reg.rushDelay;
    scriptID = reg.scriptID;
    petList = reg.petList;
    summonList = reg.summonList;
    deathSummonList = reg.deathSummonList;
    dieList = reg.dieList;
    intervalAmendment = reg.intervalAmendment;
	AmendmentFactor = reg.AmendmentFactor;
	Camp = reg.Camp;
    return *this;
  }

  void fillNpcMap(const char * tempChar,std::map<DWORD,std::pair<DWORD,DWORD> > &map)
  {
    std::vector<std::string> vs;
    std::vector<std::string> sub_vs;
    vs.clear();
    stringtok(vs,tempChar,";");
    for (DWORD i=0; i<vs.size(); i++)
    {
      sub_vs.clear();
      stringtok(sub_vs,vs[i].c_str(),"-");
      if (sub_vs.size()==2)
        map[atoi(sub_vs[0].c_str())] = std::make_pair(atoi(sub_vs[1].c_str()),100);
      else if (sub_vs.size()==3) //npcID,num,rate
        map[atoi(sub_vs[0].c_str())] = std::make_pair(atoi(sub_vs[1].c_str()),atoi(sub_vs[2].c_str()));
    }
  }

  //sky 计算复活时间修改系数
  void InitIntervalAmendment()
  {
	  //sky 复活时间为0或者数量小于1的时候
	  if(interval == 0 || num <= 1)
		  return;

	  //sky 系数取复活时间与刷怪数量的平均值(时间先转换成毫秒级)
	  intervalAmendment = (interval * 1000)/num;
	  AmendmentFactor = num;

	  return;
  }

  //sky 获取复活时间修正值
  //Dead 生命状态 (true:重生 false:死亡)
  DWORD GetIntervalAmendmaent(bool Dead)
  {
	  if(Dead)
	  {
		  //sky 如果是重生,把重生的系数提高
		  AmendmentFactor++;
		  if(AmendmentFactor > num)
			  AmendmentFactor = num;
	  }
	  else
	  {
		  //sky 如果是死亡,就把重生的系数减少
		  AmendmentFactor--;
		  if(AmendmentFactor < 0)
			  AmendmentFactor = 0;
	  }

	  //sky 返回修正值
	  return intervalAmendment * AmendmentFactor;
  }
};

typedef std::vector<t_NpcDefine> NpcDefineVector;

/**
 * \brief 处理怪物攻城的类,解析脚本并根据脚本控制攻城boss
 *
 */
class Rush
{
  private:
    //DWORD summonCount;

    /**
     * \brief boss 的定义,用于召唤boss
     * 可以定义多个boss,召唤时随机选一个
     *
     */
    struct bossDefine
    {
      ///boss编号,对应npc表
      DWORD id;
      ///出生位置
      zPos pos;
      ///出生范围
      int region;
      ///数量
      int num;
      ///脚本ID
      DWORD script;
    };
    /**
     * \brief 仆人定义
     *  仆人是boss召唤出来的小怪,协助bos攻城
     *  仆人不受Rush类控制,行为方式与普通怪物相同
     *  Rush类只保存它们的指针以便在攻城结束后删除
     *
     */
    struct servantDefine
    {
      ///仆人id,对应npc表
      DWORD id;
      ///一次召唤的数量
      int num;
      ///召唤的几率
      int rate;
      ///召唤间隔
      int interval;
    };
    ///存放多个boss,但是只召唤一个
    std::vector<bossDefine> bossVector;
    ///存放多个仆人定义
    std::vector<servantDefine> servantVector;
    ///召唤出来仆人的指针,用于删除
    std::list<SceneNpc *> servants;

    /**
     * \brief 动作定义,每个阶段有一个动作
     *
     */
    enum rushAction
    {
      ///普通
      RUSH_NORMAL,
      ///攻城之前的复活阶段
      RUSH_RELIVE,
      ///移动
      RUSH_MOVETO,
      ///攻击
      RUSH_ATTACK,
      ///召唤仆人
      RUSH_SUMMON,
      ///召唤跟随boss往城里冲的仆人
      RUSH_SUMMON_RUSH,
      ///打开召唤
      RUSH_SUMMON_ON,
      ///关闭召唤,关闭后不会再召唤仆人
      RUSH_SUMMON_OFF,
      ///清除攻城怪物,包括boss和仆人
      RUSH_CLEAR,
      ///给boss加满hp/sp/mp
      RUSH_RECOVER,
      ///召唤跟随boss的宠物
      RUSH_SUMMON_PET,
      ///世界广播
      RUSH_BROADCAST,
      ///攻城结束
      RUSH_END
    };
    /**
     * \brief 阶段定义,攻城的阶段
     * Rush类的目的就是控制boss按照攻城阶段一步步进行
     *
     */
    struct phaseDefine
    {
      ///阶段动作
      rushAction action;
      ///位置,在每个阶段意义略不相同
      ///移动时表示目的,攻击时表示活动范围中心位置
      zPos pos;
      ///范围 移动时表示到达目标的判定距离,攻击时表示活动范围大小
      int region;
      ///持续时间 一个阶段的持续时间,到时会触发onPhaseEnd事件
      int lasttime;
      ///boss的说话,每阶段开头说话
      char say[MAX_CHATINFO];
      ///3个参数
      DWORD x,y,z;
    };
    ///存放阶段列表
    std::vector<phaseDefine> phaseVector;
    ///当前的阶段索引
    DWORD curPhase;
    ///阶段结束时间
    zRTime nextPhaseTime;
    ///攻城结束的时间
    zRTime endTime;
    
    ///攻城脚本的编号
    DWORD id;
    ///目标国家的编号
    DWORD countryID;

    ///攻城boss的id
    DWORD bossID;
    ///boss指针
    SceneNpc * boss;

    ///下次可召唤的时间
    zRTime summonTime;
    ///上次循环时boss的hp
    DWORD lastBossHp;
    ///召唤仆人的开关
    bool canSummon;
    ///攻城时间结束或者boss死亡后过多久清除所有攻城npc
    int clearDelay;
    ///是否结束
    bool end;
    ///发起时的提示语
    char text[128];
    std::string startText;
    std::string endText;

    ///召唤仆人的范围,位置是以boss的当前位置为中心
    static const int summon_servant_region = 4;
    bool checkSummonTime();
    
    bool loadRushData(DWORD rushID,DWORD rushDelay,DWORD countryID);
    void onPhaseTimeOver();
    void onTimeOver();
    void setEndTime(int delay);
    bool checkEndTime();
    void setPhaseTime(const int delay);
    bool checkPhaseTime();

    void enterNextPhase();
    bool summonBoss();
    bool summonServant();
    bool summonPet();
    void deleteBoss();
    void clearServants();

    rushAction parseAction(const char *);
  public:
    ///攻城总持续时间
    int lasttime;
    ///多长时间后开始攻城,也就是boss的复活时间
    DWORD rushDelay;
    ///本次攻城的名字
    char rushName[MAX_NAMESIZE];
    ///目标地图名字
    char mapName[MAX_NAMESIZE];
    ///boss的名字
    char bossName[MAX_NAMESIZE];

    Rush(DWORD rushID,DWORD rushDelay,DWORD countryID);
    bool init(Scene * =0);
    ~Rush();
    void process();
    bool isEnd();

    void terminate();
};

/**
 * \brief 场景物件屏索引
 */
class zSceneEntryIndex : private zNoncopyable
{

  protected:

    ///场景宽和高
    zPos sceneWH;
    ///最大屏索引编号
    DWORD screenMax;
    ///横向多少屏幕
    DWORD screenx;
    ///纵向多少屏幕
    DWORD screeny;

  private:

    typedef std::set<zSceneEntry *,std::less<zSceneEntry *> > SceneEntry_SET;
    typedef std::set<SceneNpc *,std::less<SceneNpc *> > Npc_Index;

    typedef hash_map<zPosI,SceneEntry_SET> PosIMapIndex;
    typedef hash_map<DWORD,Npc_Index> SpecialNpc_Index;

    /**
     * \brief map索引容器
     */
    PosIMapIndex index[zSceneEntry::SceneEntry_MAX];
    /// 所有物件的索引
    SceneEntry_SET all[zSceneEntry::SceneEntry_MAX];
    /// 对于特殊npc的索引
    SpecialNpc_Index special_index;
    /// 对于功能npc的索引
    Npc_Index functionNpc;

    typedef std::map<DWORD,int,std::less<DWORD> > PosiEffectMap;

    typedef PosiEffectMap::iterator PosiEffectMap_iter;
    PosiEffectMap posiEffect[MAX_NPC_GROUP];

    //在加载的时候计算九屏关系并保存
    typedef hash_map<DWORD,zPosIVector> NineScreen_map;
    typedef NineScreen_map::iterator NineScreen_map_iter;
    typedef NineScreen_map::const_iterator NineScreen_map_const_iter;
    typedef NineScreen_map::value_type NineScreen_map_value_type;
    NineScreen_map ninescreen;
    NineScreen_map direct_screen[8];
    NineScreen_map reversedirect_screen[8];

    void freshEffectPosi(const zPosI oldposi,const zPosI newposi);

  public:

    /**
     * \brief 构造函数
     */
    zSceneEntryIndex() {}
    /**
     * \brief 析构函数
     */
    virtual ~zSceneEntryIndex() {}

    virtual void freshGateScreenIndex(SceneUser *pUser,const DWORD screen) = 0;

    bool refresh(zSceneEntry *e,const zPos & newPos);
    bool removeSceneEntry(zSceneEntry *e);
    void execAllOfScreen(const zPosI screen,zSceneEntryCallBack &callback);
    void execAllOfScreen(const zSceneEntry::SceneEntryType type,const zPosI screen,zSceneEntryCallBack &callback);
    void execAllOfScene(zSceneEntryCallBack &callback);
    void execAllOfScene(const zSceneEntry::SceneEntryType type,zSceneEntryCallBack &callback);
    void execAllOfScene_npc(const DWORD id,zSceneEntryCallBack &callback);
    void execAllOfScene_functionNpc(zSceneEntryCallBack &callback);

    void setSceneWH(const zPos sceneWH,const DWORD screenx,const DWORD screeny,const DWORD screenMax);
	zSceneEntry *getSceneEntryByPos(zSceneEntry::SceneEntryType type,const zPos &pos,const bool bState = true,const zSceneEntry::SceneEntryState byState = zSceneEntry::SceneEntry_Normal);

    void execAllOfEffectNpcScreen(const DWORD group,zSceneEntryCallBack &callback);

    /**
     * \brief 获取地图宽
     * 单位是格点
     * \return 地图宽
     */
    const DWORD width() const { return sceneWH.x; }
    /**
     * \brief 获取地图高
     * 单位是格点
     * \return 地图高
     */
    const DWORD height() const { return sceneWH.y; }
    /**
     * \brief 获取地图宽
     * 单位是屏
     * \return 地图宽
     */
    const DWORD getScreenX() const { return screenx; }
    /**
     * \brief 获取地图高
     * 单位是屏
     * \return 地图高
     */
    const DWORD getScreenY() const { return screeny; }
    /**
     * \brief 坐标转化
     * 绝对坐标转化为屏编号相对坐标
     * \param p 绝对坐标系统 
     * \param pi 相对坐标系统
     * \return 返回转化后的相对坐标系统
     */
    zPosI &zPos2zPosI(const zPos &p,zPosI &pi) const
    {
      zSceneEntry::zPos2zPosI(sceneWH,p,pi);
      return pi;
    }
    /**
     * \brief 重新计算坐标，保证不越界
     * \param pos 待验证的坐标
     */
    const void zPosRevaluate(zPos &pos) const
    {
      if (pos.x & 0x80000000)
        pos.x = 0;
      if (pos.y & 0x80000000)
        pos.y = 0;
      if (pos.x >= sceneWH.x)
        pos.x = sceneWH.x - 1;
      if (pos.y >= sceneWH.y)
        pos.y = sceneWH.y - 1;
    }
    /**
     * \brief 验证坐标是否合法，是否越界
     * \param pos 坐标
     * \return 是否合法
     */
    const bool zPosValidate(const zPos &pos) const { return pos.x < sceneWH.x && pos.y < sceneWH.y; }
    /**
     * \brief 验证坐标是否合法，是否越界
     * \param posi 坐标
     * \return 是否合法
     */
    const bool zPosIValidate(const zPosI &posi) const { return posi < screenMax; }
    /**
     * \brief 检查两个坐标编号是否有9屏关系
     * \param one 第一个编号
     * \param two 第二个编号
     * \return 是否有9屏关系
     */
    bool checkTwoPosIInNine(const zPosI one,const zPosI two) const
    {
      int oneX,oneY,twoX,twoY;
      oneX = one % screenx;
      oneY = one / screenx;
      twoX = two % screenx;
      twoY = two / screenx;
      if (abs(oneX - twoX) <=1 && abs(oneY - twoY) <=1)
      {
        return true;
      }
      return false;
    }
    /**
     * \brief 判断两点之间距离是否超出了半径
     * \param pos1 起点
     * \param pos2 终点
     * \param radius 半径
     * \return 是否超出范围
     */
    const bool zPosShortRange(const zPos &pos1,const zPos &pos2,const int radius) const
    {
      if (abs((long)(pos1.x - pos2.x)) <= radius
          && abs((long)(pos1.y - pos2.y)) <= radius)
        return true;
      else
        return false;
    }
    /**
     * \brief 判断两点之间距离是否超出了矩形范围
     * \param pos1 起点
     * \param pos2 终点
     * \param wide 矩形宽
     * \param height 矩形高
     * \return 是否超出范围
     */
    const bool zPosShortRange(const zPos &pos1,const zPos &pos2,const int wide,const int height) const
    {
      if (abs((long)(pos1.x - pos2.x)) <= wide
          && abs((long)(pos1.y - pos2.y)) <= height)
        return true;
      else
        return false;
    }
    /*
     * \brief 得到两点的距离
     * 这个距离只是作为比较判断，不是作为实际的距离
     * \param pos1 位置1
     * \param pos2 位置2
     * \return 距离
     */
    DWORD getDistance(zPos pos1,zPos pos2) const { return abs((long)(pos1.x-pos2.x))+abs((long)(pos1.y-pos2.y)); }
    /**
     * \brief 方向取反
     * \param direct 方向
     * \return 相反的方向
     */
    int getReverseDirect(const int direct) const { return (direct + 4) % 8; }
    /**
     * \brief 根据两屏判断九个方向
     * \param posiorg 原点屏编号
     * \param posinew 结束点屏编号
     * \return 方向编号，如果为-1表示不是九个方向之一
     */
    int getScreenDirect(const zPosI posiorg,const zPosI posinew) const
    {
      using namespace Cmd;
      DWORD orgX = posiorg % screenx;
      DWORD orgY = posiorg / screenx;
      DWORD newX = posinew % screenx;
      DWORD newY = posinew / screenx;
      if (orgX == newX && orgY == newY+1)
      {
        return _DIR_UP;
      }
      else if (orgX + 1 == newX && orgY == newY +1)
      {
        return _DIR_UPRIGHT;
      }
      else if (orgX +1 == newX && orgY == newY)
      {
        return _DIR_RIGHT;
      }
      else if (orgX +1 == newX && orgY +1 == newY)
      {
        return _DIR_RIGHTDOWN;
      }
      else if (orgX == newX && orgY +1 == newY)
      {
        return _DIR_DOWN;
      }
      else if (orgX == 1 + newX && orgY +1 == newY)
      {
        return _DIR_DOWNLEFT;
      }
      else if (orgX == 1 + newX && orgY == newY)
      {
        return _DIR_LEFT;
      }
      else if (orgX == 1 + newX && orgY == 1 + newY)
      {
        return _DIR_LEFTUP;
      }
      else
        return _DIR_WRONG;
    }

    /**
     * \brief 查找我在另一个坐标点的大致方向
     * \param myPos 我的位置
     * \param otherPos 对方的位置
     * \return 我在otherPos的相对方向
     */
    static int getCompDir(const zPos &myPos,const zPos &otherPos)
    {
      int x = otherPos.x - myPos.x;
      int y = otherPos.y - myPos.y;
      int absx = abs(x);
      int absy = abs(y);
      if (absx > absy && absy < absx/2)
        y = 0;
      else if (absx < absy && absx < absy/2)
        x = 0;

      int bkDir = 0;

      if (x < 0 &&  y < 0)
        bkDir = 7;
      else if (x < 0 && 0 == y)
        bkDir = 6;
      else if (x < 0 && y > 0)
        bkDir = 5;
      else if (0 == x && y > 0)
        bkDir = 4;
      else if (x > 0 && y > 0)
        bkDir = 3;
      else if (x > 0 && 0 == y)
        bkDir = 2;
      else if (x > 0 && y < 0)
        bkDir = 1;
      else if (0 == x && y < 0)
        bkDir = 0;
      return bkDir;
    }
    const zPosIVector &getScreenByRange(const zPos &pos,const int range);
    /**
     * \brief 以中心屏获取周围9屏的屏幕编号
     * \param posi 中心屏
     * \param pv 输出的屏编号向量
     */
    const zPosIVector &getNineScreen(const zPosI &posi)
    {
      NineScreen_map_const_iter iter = ninescreen.find((DWORD)posi);
      if (iter != ninescreen.end())
      {
        return iter->second;
      }
      //如果出现异常返回0编号的屏索引
      return ninescreen[(DWORD)-1];
    }

    /**
     * \brief 以中心屏获取向前的3屏或者5屏的屏幕编号
     * \param posi 中心屏
     * \param direct 方向
     * \return 输出的屏编号向量
     */
    const zPosIVector &getDirectScreen(const zPosI &posi,const int dir)
    {
      NineScreen_map_const_iter iter = direct_screen[dir].find((DWORD)posi);
      if (iter != direct_screen[dir].end())
      {
        return iter->second;
      }
      //如果出现异常返回0编号的屏索引
      return direct_screen[dir][(DWORD)-1];
    }

    /**
     * \brief 以中心屏获取向后的3屏或者5屏的屏幕编号
     * \param posi 中心屏
     * \param direct 方向
     * \return 输出的屏编号向量
     */
    const zPosIVector &getReverseDirectScreen(const zPosI &posi,const int dir)
    {
      NineScreen_map_const_iter iter = reversedirect_screen[dir].find((DWORD)posi);
      if (iter != reversedirect_screen[dir].end())
      {
        return iter->second;
      }
      //如果出现异常返回0编号的屏索引
      return reversedirect_screen[dir][(DWORD)-1];
    }

};

enum
{
	MAP_FUNCTION_NORIDE		= 0x1,	//sky 不可以骑坐骑
	MAP_FUNCTION_CAPITAL	= 0x2,	//sky 首都
	MAP_FUNCTION_MAZE		= 0x4,	//sky 地下城
	MAP_FUNCTION_NOSCROLL	= 0x8,	//sky 不可以使用卷轴
	MAP_FUNCTION_NORED		= 0x10,	//sky 不红名
	MAP_FUNCTION_NOVICE		= 0x20,	//sky 新手保护
	MAP_FUNCTION_NOTEAM		= 0x40, //sky 不可以组队
	MAP_FUNCTION_NOCALLOBJ	= 0x80, //sky 不可以使用令牌
	MAP_FUNCTION_BATTLEFIEL	= 0x100,//sky 战场类地图

};

/**
 * \brief 地图场景
 */
class Scene:public zScene,public zSceneEntryIndex
{

  protected:

    Scene();

  public:

    //由Session控制刷新的npc
    std::map<DWORD,SceneNpc *> bossMap;

    /**
     * \brief 场景类型定义
     */
    enum SceneType
    {
      STATIC,   /// 静态地图
      GANG    /// 动态地图
    };

    virtual ~Scene();
    virtual bool save() =0;

    bool init(DWORD countryid,DWORD mapid);
    void final();
    void freshGateScreenIndex(SceneUser *pUser,const DWORD screen);

    bool sendCmdToWatchTrap(const zPosI screen,const void *pstrCmd,const int nCmdLen);
    bool sendCmdToNine(const zPosI screen,const void *pstrCmd,const int nCmdLen,unsigned short dupIndex = 0);
    bool sendCmdToNineUnWatch(const zPosI screen,const void *pstrCmd,const int nCmdLen);
    bool sendCmdToScene(const void *pstrCmd,const int nCmdLen,unsigned short dupIndex = 0);
    bool sendCmdToNineExceptMe(zSceneEntry *pEntry,const zPosI screen,const void *pstrCmd,const int nCmdLen);
    bool sendCmdToDirect(const zPosI screen,const int direct,const void *pstrCmd,const int nCmdLen,unsigned short dupIndex = 0);
    bool sendCmdToReverseDirect(const zPosI screen,const int direct,const void *pstrCmd,const int nCmdLen,unsigned short dupIndex = 0);
    bool findEntryPosInNine(const zPos &vpos,zPosI vposi,zPosVector &range);
    bool findEntryPosInOne(const zPos &vpos,zPosI vposi,zPosVector &range);

    int summonNpc(const t_NpcDefine &define,const zPos &pos,zNpcB *base,unsigned short dupIndex = 0);
    template <typename Npc>
    Npc* summonOneNpc(const t_NpcDefine &define,const zPos &pos,zNpcB *base,unsigned short dupIndex,DWORD standTime=0,zNpcB* abase = NULL,BYTE vdir=4, SceneEntryPk * m = NULL );

    bool refreshNpc(SceneNpc *sceneNpc,const zPos & newPos);

    /**
     * \brief 获取重生地图
     * \return 重生地图ID
     */
    DWORD backtoMap() const { return backtoMapID; }
    /**
     * \brief 获取回城地图
     * \return 回城地图ID
     */
    DWORD backtoCityMap() const { return backtoCityMapID; }
    /**
     * \brief 允许相互pk的等级
     * \return 是true
     */
    DWORD getPkLevel() const { return pklevel; }
    /**
     * \brief 检测该地图是否可以骑马
     * \return 可骑马时true
     */
    bool canRide() const { return !(function & MAP_FUNCTION_NORIDE); }
    /**
     * \brief 不能使用卷轴
     * \return 是主城返回true
     */
    bool canUserScroll() const { return !(function & MAP_FUNCTION_NOSCROLL); }
    /**
     * \brief 检测该地图是否可以组队
     * \return 可组队时true
     */
    bool noTeam() const { return (function & MAP_FUNCTION_NOTEAM); }
    /**
     * \brief 检测该地图是否可以使用令牌
     * \return 是否可用
     */
    bool checkCallObj() const { return (function & MAP_FUNCTION_NOCALLOBJ); }
    /**
     * \brief 是否是主城
     * \return 是主城返回true
     */
    bool isMainCity() const { return (function & MAP_FUNCTION_CAPITAL); }
    /**
     * \brief 是否是地洞类
     * \return 是主城返回true
     */
    bool isField() const { return (function & MAP_FUNCTION_MAZE); }
    /**
     * \brief sky 修改为新手保护地图
     * \return 是新手保护地图返回true
     */
    bool isNovice() const { return (function & MAP_FUNCTION_NOVICE); }
    /**
     * \brief 是否是不红名地图
     * \return 是true
     */
    bool isNoRedScene() const { return (function & MAP_FUNCTION_NORED); }
    /**
     * \brief 是否是PK地图
     * \return 是true
     */
    bool isPkMap() const { return getRealMapID()>=213 && getRealMapID()<=215; }
    /**
     * \brief 获取地图编号
     * \return 地图编号
     */
    const DWORD getRealMapID() const { return id & 0x0000FFFF; }
    /**
     * \brief 获取地图所属国家
     * \return 地图所属国家
     */
    const DWORD getCountryID() const { return countryID; }

    const char *getCountryName() const;

    /**
     * \brief 获取地图名称
     * \return 地图名称(用于服务间)
     */
    const char *getName() const { return name; }
    /**
     * \brief 获取地图名称
     * \return 地图名称(未进行组合的名称)
     */
    const char *getRealName() const
    {
      char *real = const_cast<char*>(strstr(name,"·"));
      if (real != NULL)
        return real + 2;
      else
        return name;
    }
    /**
     * \brief 获取地图文件名称
     * 名称不包括前缀
     * \return 地图文件名称(用于服务间)
     */
    const char *getFileName() const { return fileName.c_str(); }
    /**
     * \brief 获取地图文件名称
     * 名称不包括前缀
     * \return 地图文件名称(为进行组合的名称)
     */
    const char *getRealFileName() const { return fileName.c_str() + fileName.find(".") + 1; }
    /**
     * \brief 获取指定地图文件名称
     * 名称不包括前缀
     * \param file 文件名
     * \return 地图文件名称(为进行组合的名称)
     */
    const char *getRealFileName(std::string file) const { return file.c_str() + file.find(".") + 1; }
    /**
     * \brief 检查坐标阻挡信息
     * \param pos 坐标
     * \param block 阻挡标记
     * \return 是否阻挡点
     */
    const bool checkBlock(const zPos &pos,const BYTE block) const
    {
      if (zPosValidate(pos))
        return (0 != (allTiles[pos.y * width() + pos.x].flags & block));
      else
        return true;
    }
    /**
     * \brief 检查坐标阻挡信息
     * \param pos 坐标
     * \return 是否阻挡点
     */
    const bool checkBlock(const zPos &pos) const { return checkBlock(pos,TILE_BLOCK | TILE_ENTRY_BLOCK); }
    /**
     * \brief 设置目标阻挡点标记
     * \param pos 坐标
     * \param block 阻挡标记
     */
    void setBlock(const zPos &pos,const BYTE block)
    {
      if (zPosValidate(pos))
        allTiles[pos.y * width() + pos.x].flags |= block;
    }
    /**
     * \brief 设置目标阻挡点标记
     * \param pos 坐标
     */
    void setBlock(const zPos &pos) { setBlock(pos,TILE_ENTRY_BLOCK); }

	// [ranqd] 设置区域阻挡点标记
	void setBlock(const zZone &zone,const BYTE block)
	{
		for(int x = zone.pos.x; x < zone.pos.x + zone.width; x++)
		{
			for( int y = zone.pos.y; y < zone.pos.y + zone.height; y++)
			{
				zPos pos;
				pos.x = x;
				pos.y = y;
				if (zPosValidate(pos))
					allTiles[pos.y * width() + pos.x].flags |= block;
			}
		}
	} 

	void setBlock(const zZone &pos) { setBlock(pos,TILE_ENTRY_BLOCK); }
    /**
     * \brief 清除目标阻挡点标记
     * \param pos 坐标
     * \param block 阻挡标记
     */
    void clearBlock(const zPos &pos,const BYTE block)
    {
      if (zPosValidate(pos))
        allTiles[pos.y * width() + pos.x].flags &= ~block;
    }
    /**
     * \brief 清除目标阻挡点标记
     * \param pos 坐标
     */
    void clearBlock(const zPos &pos) { clearBlock(pos,TILE_ENTRY_BLOCK); }
    /**
     * \brief 检查坐标阻挡信息
     * 主要在丢物品的时候使用
     * \param pos 坐标
     * \return 是否阻挡点
     */
    const bool checkObjectBlock(const zPos &pos) const { return checkBlock(pos,TILE_BLOCK | TILE_OBJECT_BLOCK); }
    /**
     * \brief 设置目标阻挡点标记
     * 主要在丢物品的时候使用
     * \param pos 坐标
     */
    void setObjectBlock(const zPos &pos) { setBlock(pos,TILE_OBJECT_BLOCK); }
    /**
     * \brief 清除目标阻挡点标记
     * 主要在丢物品的时候使用
     * \param pos 坐标
     */
    void clearObjectBlock(const zPos &pos) { clearBlock(pos,TILE_OBJECT_BLOCK); }
    /**
     * \brief 获取地表数据
     * \param pos 坐标
     * \return 返回地表数据
     */
    const Tile* getTile(const zPos &pos) const
    {
      if (zPosValidate(pos))
        return &allTiles[pos.y * width() + pos.x];
      else
        return NULL;
    }
    /**
     * \brief 根据位置得到路点
     * \param pos 要查找的位置
     * \return 找到的路点,失败返回0
     */
    WayPoint *getWayPoint(const zPos &pos) { return wpm.getWayPoint(pos); }
    /**
     * \brief 根据目标找到路点
     * \param filename 目标地图文件名
     * \return 找到的路点,失败返回0
     */
    WayPoint *getWayPoint(const char *filename) { return wpm.getWayPoint(filename); }
    /**
     * \brief 随机选一个路点
     * \return 找到的路点
     */
    WayPoint *getRandWayPoint() { return wpm.getRandWayPoint(); }
    /**
     * \brief 返回地图上的人数
     * \return 一张地图上的人数
     */
    const DWORD countUser() const { return userCount; }
    /**
     * \brief 人数增加
     * \return 增加后的人数
     */
    const DWORD addUserCount() { return ++userCount; }
    /**
     * \brief 人数减少
     * \return 减少后的人数
     */
    const DWORD subUserCount() { return --userCount; }

    bool findPosForObject(const zPos &pos,zPos &findedPos);
    bool findPosForUser(const zPos &pos,zPos &findedPos);
    SceneUser *getSceneUserByPos(const zPos &pos,const bool bState = true,const zSceneEntry::SceneEntryState byState = zSceneEntry::SceneEntry_Normal);
    SceneUser *getUserByID(DWORD userid);
    SceneUser *getUserByTempID(DWORD usertempid);
    SceneUser *getUserByName(const char *username);
    SceneNpc *getSceneNpcByPos(const zPos &pos,const bool bState = true,const zSceneEntry::SceneEntryState byState = zSceneEntry::SceneEntry_Normal);
    SceneNpc *getNpcByTempID(DWORD npctempid);
    zSceneObject *getSceneObjectByPos(const zPos &pos);

    bool addObject(unsigned short dupIndex,zObject *ob,const zPos &pos,const unsigned long overdue_msecs=0,const unsigned long dwID=0,int protime=10);
    bool addObject(unsigned short dupIndex,zObjectB *ob,const int num,const zPos &pos,const unsigned long dwID=0,DWORD npc_mul=0, DWORD teamID=0);
    void removeUser(SceneUser *so);
    void removeObject(zSceneObject *so);
    void removeNpc(SceneNpc *sn);
    bool checkZoneType(const zPos &pos,const int type) const;
    int getZoneType(const zPos &pos) const;
    bool randzPosByZoneType(const int type,zPos &pos) const;
    int changeMap(SceneUser *pUser,bool deathBackto=true,bool ignoreWar=false);
    bool randzPosByZoneType(const int type,zPos &pos,const zPos orign);
    bool randzPosOnRect(const zPos &center,zPos &pos,WORD rectx = SCREEN_WIDTH,WORD recty = SCREEN_HEIGHT) const;

    bool getNextPos(int &side,const int direct,const zPos &orgPos,const int clockwise,zPos &crtPos) const;
    void getNextPos(const zPos &orgPos,const int dir,zPos &newPos) const;
    bool SceneEntryAction(const zRTime& ctv,const DWORD group);
    void removeSceneObjectInOneScene();

    bool addRush(Rush *);
    void processRush();

    void setUnionDare(bool flag){this->isUnionDare = flag;}
    bool getUnionDare() const { return this->isUnionDare; }
    void setHoldUnion(DWORD dwUnionID) { this->dwHoldUnionID = dwUnionID; }
    DWORD getHoldUnion() const { return this->dwHoldUnionID; }
    void setHoldCountry(DWORD dwCountryID) { this->dwHoldCountryID = dwCountryID; }
    DWORD getHoldCountry() const { return this->dwHoldCountryID; }
    DWORD getCommonCountryBacktoMapID() const { return this->commonCountryBacktoMapID; }
    DWORD getForeignerBacktoMapID() const { return this->foreignerBacktoMapID; }
    DWORD getCountryDareBackToMapID() const { return this->countryDareBackToMapID; }
    DWORD getCountryDefBackToMapID() const { return this->countryDefBackToMapID; }
    DWORD getCommonUserBacktoMapID() const { return this->commonUserBacktoMapID; }
    void setCountryDare(bool flag) { this->isCountryFormalDare = flag; }
    bool getCountryDare() const { return this->isCountryFormalDare; }
    void setEmperorDare(bool flag,DWORD dwDefCountryID) { this->isEmperorDare = flag; this->dwEmperorDareDef = dwDefCountryID; }
    void reliveSecGen();
    bool getEmperorDare() const { return this->isEmperorDare; }
    DWORD getEmperorDareDef() const { return this->dwEmperorDareDef; }
    void setTax(DWORD dwTax) { countryTax = dwTax; }
    const DWORD getTax() const { return countryTax; }
    void addFieldMapName(const char *name)
    {
      stMapName mapname;
      strncpy_s(mapname.strMapName,name,MAX_NAMESIZE);
      fieldMapName.push_back(mapname);
    }
    void clearFieldMapName() { fieldMapName.clear(); }
    DWORD getField(const char *ou) const
    {
      stMapName *out = (stMapName *)ou; 
      int i=0;
      for(std::vector<stMapName>::const_iterator iter = fieldMapName.begin();iter!=fieldMapName.end();iter++)
      {
        strncpy_s((&out[i])->strMapName,(*iter).strMapName,MAX_NAMESIZE);
        i++;
      }
      return i;
    }
    bool checkField(const char *out) const
    {
      for(std::vector<stMapName>::const_iterator iter = fieldMapName.begin();iter!=fieldMapName.end();iter++)
      {
        if (strncmp((char *)out,(*iter).strMapName,MAX_NAMESIZE) == 0)
        {
          return true;
        }
      }
      return false;
    }
    void addMainMapName(const char *name)
    {
      stMapName mapname;
      strncpy_s(mapname.strMapName,name,MAX_NAMESIZE);
      mainMapName.push_back(mapname);
    }

    void clearMainMapName() { mainMapName.clear(); }
    DWORD getMainCity(const char *ou) const
    {
      stMapName *out = (stMapName *)ou; 
      int i=0;
      for(std::vector<stMapName>::const_iterator iter = mainMapName.begin();iter!=mainMapName.end();iter++)
      {
        strncpy_s((&out[i])->strMapName,(*iter).strMapName,MAX_NAMESIZE);
        i++;
      }
      return i;
    }
    bool checkMainCity(const char *out) const
    {
      for(std::vector<stMapName>::const_iterator iter = mainMapName.begin();iter!=mainMapName.end();iter++)
      {
        if (strncmp((char *)out,(*iter).strMapName,MAX_NAMESIZE) == 0)
        {
          return true;
        }
      }
      return false;
    }

    void addIncMapName(const char *name)
    {
      stMapName mapname;
      strncpy_s(mapname.strMapName,name,MAX_NAMESIZE);
      incMapName.push_back(mapname);
    }

    void clearIncMapName() { incMapName.clear(); }
    DWORD getIncCity(const char *ou) const
    {
      stMapName *out = (stMapName *)ou; 
      int i=0;
      for(std::vector<stMapName>::const_iterator iter = incMapName.begin();iter!=incMapName.end();iter++)
      {
        strncpy_s((&out[i])->strMapName,(*iter).strMapName,MAX_NAMESIZE);
        i++;
      }
      return i;
    }
    bool checkIncCity(const char *out) const
    {
      for(std::vector<stMapName>::const_iterator iter = incMapName.begin();iter!=incMapName.end();iter++)
      {
        if (strncmp((char *)out,(*iter).strMapName,MAX_NAMESIZE) == 0)
        {
          return true;
        }
      }
      return false;
    }


    DWORD sceneExp(DWORD exp) const { return (DWORD)(exp * exprate); }
    DWORD winnerExp(DWORD exp) const
    {
      if (winner_exp)
      {
        return (DWORD)(exp * 0.5f);
      }
      else
      {
        return 0;
      }
    }
    BYTE getLevel() const { return level; }
    bool checkUserLevel(SceneUser *pUser);
    ///战胜国经验加成标志 
    bool winner_exp; 

    /// 国战战场死亡后,攻方死亡复活地
    DWORD countryDareBackToMapID;


    /**
     * \brief 是否是收费地图
     * \return 是否是收费地图
     */
    bool isTrainingMap()
    {
      DWORD i = id & 0x0000FFFF;
      return (i>=193 && i<=202);
    }
    void initRegion(zRegion &reg,const zPos &pos,const WORD width,const WORD height);

    bool randPosByRegion(const zPosIndex &index,zPos &pos) const;
  public:
    struct stMapName
    {
      stMapName()
      {
        bzero(strMapName,sizeof(strMapName));
      }
      char strMapName[MAX_NAMESIZE];
    };
    //练功点地图
    std::vector<stMapName> fieldMapName;
    //主城地图
    std::vector<stMapName> mainMapName;
    //增值地图
    std::vector<stMapName> incMapName;
    std::map<std::string,std::string> params;

    ///给npc分组处理
    DWORD execGroup;

    /// 帮会夺城战进行标志
    bool isUnionDare;
    
    /// 该场景所属帮会
    DWORD dwHoldUnionID;

    /// 该场景占领者国家ID
    DWORD dwHoldCountryID;


    /// 正式国战正在该场景进行的标志
    bool isCountryFormalDare;

    /// 皇城战正在该场景进行的标志
    bool isEmperorDare;
    
    /// 皇城战的守方
    DWORD dwEmperorDareDef;

    ///本场景的攻城列表
    std::list<Rush *> rushList;

    ///一秒定时器
    Timer _one_sec;

    ///已经初始化
    bool inited;

    ///所有的地图格子
    zTiles allTiles;
    ///所有的npc定义
    NpcDefineVector npcDefine;
    ///场景中各种区域的定义
    ZoneTypeDefVector zoneTypeDef;

    ///本场景用户数
    DWORD userCount;
    ///如果本地图没有重生区时需要跳转到的地图
    DWORD backtoMapID;
    ///回到主城的地图id
    DWORD backtoCityMapID;
    ///回到国战目的地
    DWORD backtoDareMapID;
    /// 外国人死亡后应该回到的地图id(没有国家信息)
    DWORD foreignerBacktoMapID;
    /// 在公共国死亡后应该回到的地图id(没有国家信息)
    DWORD commonCountryBacktoMapID; 
    /// 无国籍人在外国死亡重生地
    DWORD commonUserBacktoMapID; 
    /// 国战战场死亡后,守方死亡复活地
    DWORD countryDefBackToMapID;
    ///地图特殊说明
    DWORD function;
    /// 可相互pk的等级
    DWORD pklevel;
    ///国家id
    DWORD countryID;
    ///场景对应的文件名
    std::string fileName;
    ///路点管理器
    WayPointM wpm;
    ///本地图收取税费
    DWORD countryTax;
    ///本地图允许进入的最小玩家等级
    BYTE level;
    ///场景地图加成
    float exprate;

    struct FixedRush
    {
      DWORD id;//ID
      DWORD nextTime;//下次的时间
      DWORD allStart;//总开始时间
      DWORD allEnd;//总结束时间
      int weekDay;//星期几
      tm startTime;//一天中开始的时间
      tm endTime;//一天中结束的时间
      DWORD delay;//开始延迟

      FixedRush()
      {
        id = 0;//ID
        nextTime = 0;//下次的时间
        allStart = 0;//总开始时间
        allEnd = 0;//总结束时间
        weekDay = 0;//星期几
        delay = 0;//开始延迟
      }
    } fixedRush;

	//sky 战场规则文件配置相对路径
	char DulesFileName[MAX_PATH];

    bool initWayPoint(zXMLParser *parser,const xmlNodePtr node,DWORD countryid);
    bool loadMapFile();
    void initNpc(SceneNpc *sceneNpc,zRegion *init_region,zPos myPos=zPos(0,0));
    bool initByNpcDefine(const t_NpcDefine *pDefine);

    void runCircle_anti_clockwise(const int side,const DWORD X,const DWORD Y,DWORD &CX,DWORD &CY) const;
    void runCircle_clockwise(const int side,const DWORD X,const DWORD Y,DWORD &CX,DWORD &CY) const;

    void updateSceneObject();

    zPosIndex _index;  /// 非阻挡点索引
public:
	//sky 新增战场类场景虚函数
	virtual bool IsGangScene() { return false; }
	virtual void GangSceneTime(const zRTime& ctv) { return; }
	virtual DWORD ReCampThisID(BYTE index) { return 0; }
	//sky 新增战场类场景虚函数 end
};

/**
 * \brief 静态场景
 *
 */
class StaticScene:public Scene
{
  public:
    StaticScene();
    ~StaticScene();
    bool save();
	virtual bool IsGangScene() { return false; }
};


/**
 * \brief 对每个scene执行的回调基类
 *
 */
struct SceneCallBack
{
  virtual bool exec(Scene *scene)=0;
  virtual ~SceneCallBack(){};
};

/**
 * \brief 场景管理器
 *
 */
class SceneManager
:public zSceneManager
{
  public:
    typedef std::vector<std::pair<DWORD,DWORD> > NewZoneVec;
    typedef NewZoneVec::iterator newZoneVec_iter;
  public:

	  	DWORD getMapId(DWORD countryid,DWORD mapid);
  
private:
    ///唯一实例
    static SceneManager * sm;
    ///唯一ID分配器
    zUniqueDWORDID *sceneUniqeID;

    ///是否已初始化
    bool inited;

    SceneManager();
    ~SceneManager();

    bool getUniqeID(DWORD &tempid);
    void putUniqeID(const DWORD &tempid);

	///sky 队伍唯一ID分配器
	zUniqueDWORDID *sceneTeamID;

	//sky DWORD1 队伍唯一ID  
	//TeamManager 队伍对象
	std::map<DWORD, TeamManager*> ScenTeamMap;

    bool newzone;
    NewZoneVec newzon_vec; 

  public:
    static SceneManager &getInstance();
    static void delInstance();
    bool init();
    void final();

    NewZoneVec &queryNewZonePos();
    bool isNewZoneConfig();
    bool randzPosNewZone(Scene *intoScene,zPos &findedPos);
    void addNewZonePos(DWORD x,DWORD y); 
    void setNewZoneConfig(bool type);
    Scene * getSceneByFileName( const char * name);
    Scene * getSceneByName( const char * name);
    Scene * getSceneByTempID( DWORD tempid);
    Scene * getSceneByID( DWORD id);
    Scene * loadScene(/*Scene::SceneType type*/int type,DWORD countryid,DWORD mapid);
	//sky 新增战场动态地图生成
	Scene * loadBattleScene(DWORD baseid);
	//sky end
    void unloadScene(std::string &name);
    void unloadScene(Scene * &scene);
    void unloadAllScene();

    void freshEverySceneField();
    void execEveryScene(SceneCallBack &callback);
    //void execEveryMap(MapCallBack &callback);
    template <class YourEntry>
    void execEveryMap(execEntry<YourEntry> &callback)
    {
      SceneManager::MapMap_iter map_iter = SceneManager::getInstance().map_info.begin();
      for(; map_iter != SceneManager::getInstance().map_info.end() ; map_iter ++)
      {
        callback.exec(&map_iter->second);
      }
    }
    
    void checkUnloadOneScene();

	//sky 队伍管理
	///sky 操作队伍唯一ID分配器的方法
	bool getTeamID(DWORD &tempid);
	void putTeamID(const DWORD &tempid);

	//sky 根据队伍的唯一ID返回队伍对象指针
	TeamManager* GetMapTeam(DWORD TeamID);
	bool SceneNewTeam(SceneUser *pUser);		//sky 新建队伍
	bool SceneNewTeam(Cmd::Session::t_Team_Data* send);	//sky 新建队伍(跨场景专用)
	//sky 删除当前场景的队伍管理器里的队伍 同时通知Session也删除队伍管理器的队伍
	bool SceneDelTeam(DWORD TeamID);
	//sky 只删除当前场景的队伍管理器里的队伍
	bool DelMapTeam(DWORD TeamID);
	//sky 遍历队伍管理器处理roll事宜
	void TeamRollItme();
	//sky 队伍 end

    /**
     * \brief 国家信息
     *
     */
    struct CountryInfo
    {
      ///编号
      DWORD id;
      ///名字
      char name[MAX_NAMESIZE];
      ///所在的地图名字
      DWORD mapid;
      ///国家功能标识字段
      DWORD function;
    };
    /**
     * \brief 地图信息
     *
     */
    struct MapInfo
    {
      ///编号
      DWORD id;
      ///名字
      char name[MAX_NAMESIZE];
      ///对应的地图文件名
      char filename[MAX_NAMESIZE];
      ///玩家在该地图死后回到的地图
      DWORD backto;
      ///玩家在该地图死后回到的城市
      DWORD backtoCity;
      ///玩家在外国地图死后回到的城市
      DWORD foreignbackto;
      ///玩家在公共国地图死后回到的城市
      DWORD commoncountrybackto;
      ///无国家人在外国地图死后回到的城市
      DWORD commonuserbackto;
      ///国战目的地
      DWORD backtodare;
      ///国战期间,在国战战场（目前在王城）死亡后,攻方复活城市
      DWORD countrydarebackto;
      ///国战期间,在国战战场（目前在王城）死亡后,守方复活城市
      DWORD countrydefbackto;
      ///可相互pk的等级
      DWORD pklevel;
      ///表示该地图某些功能是否可用,骑马等
      DWORD function;
      ///表示该地图允许的level级别的玩家进入
      BYTE level;
      ///表示该地图的经验加成(使用时/100求出比率)
      BYTE exprate;
    };
    typedef std::map<DWORD,CountryInfo> CountryMap;
    typedef CountryMap::iterator CountryMap_iter;
    typedef CountryMap::value_type CountryMap_value_type;
    ///国家信息和ID的映射
    CountryMap country_info;
    DWORD getCountryIDByCountryName(const char *name);
    const char * getCountryNameByCountryID(DWORD);
    DWORD getMapIDByMapName(const char *name);
    const char *getMapMapNameByMapID(DWORD mapid);
    DWORD buildMapID(DWORD countryid,DWORD mapid);
    bool buildMapName(DWORD countryid,const char *in,char *out);
    bool buildMapName(DWORD countryid,DWORD mapid,char *out);
    typedef std::map<DWORD,MapInfo> MapMap;
    typedef MapMap::value_type MapMap_value_type;
    typedef MapMap::iterator MapMap_iter;
    ///地图信息和ID的映射
    MapMap map_info;
};

/**
 * \brief 定义场景服务类
 *
 * 场景服务器，游戏绝大部分内容都在本实现<br>
 * 这个类使用了Singleton设计模式，保证了一个进程中只有一个类的实例
 *
 */
class ScenesService : public zSubNetService
{

  public:
    int writeBackTimer;
    int ExpRate;
    int DropRate;
    int DropRateLevel;
    bool msgParse_SuperService(const Cmd::t_NullCmd *pNullCmd,const DWORD nCmdLen);
    BYTE countryPower[13];

    /**
     * \brief 虚析构函数
     *
     */
    virtual ~ScenesService()
    {
      instance = NULL;

      //关闭线程池
      if (taskPool)
      {
        taskPool->final();
        SAFE_DELETE(taskPool);
      }
    }

    const int getPoolSize() const
    {
      if (taskPool)
      {
        return taskPool->getSize();
      }
      else
      {
        return 0;
      }
    }

    /**
     * \brief 返回唯一的类实例
     *
     * \return 唯一的类实例
     */
    static ScenesService &getInstance()
    {
      if (NULL == instance)
        instance = new ScenesService();

      return *instance;
    }

    /**
     * \brief 释放类的唯一实例
     *
     */
    static void delInstance()
    {
      SAFE_DELETE(instance);
    }

    void reloadConfig();
    void checkAndReloadConfig();
    bool isSequeueTerminate() 
    {
      return taskPool == NULL;
    }

    //GM_logger
    static zLogger* gmlogger;
    //物品log
    static zLogger* objlogger;
    //外挂_logger
    static zLogger* wglogger;

    static Cmd::stChannelChatUserCmd * pStampData;
    static DWORD updateStampData();
  private:

    /**
     * \brief 类的唯一实例指针
     *
     */
    static ScenesService *instance;
    /**
     * \brief 设置重新读取配置标志
     *
     */
    static bool reload;

    zTCPTaskPool *taskPool;        /**< TCP连接池的指针 */

    /**
     * \brief 构造函数
     *
     */
    ScenesService() : zSubNetService("场景服务器",SCENESSERVER)
    {
      writeBackTimer = 0;
      taskPool = NULL;
    }

    bool init();
    void newTCPTask(const SOCKET sock,const struct sockaddr_in *addr);
    void final();

	//[Shx Add 读取套装配置列表 SuitInfo.xml] 
	bool LoadSuitInfo();
	//sky 读取物品冷却配置列表
	bool LoadItmeCoolTime();
	//sky 冷却配置封包函数
	bool StlToSendData();
};

/**
 * \brief 定义小游戏服务器连接客户端类
 *
 */
class MiniClient : public zTCPBufferClient
{

  public:
    
    MiniClient(
        const std::string &name,
        const std::string &ip,
        const WORD port,
        const WORD serverid)
      : zTCPBufferClient(name,ip,port) 
      {
        wdServerID=serverid;
      };

    bool connectToMiniServer();
    void run();
    bool msgParse(const Cmd::t_NullCmd *pNullCmd,const DWORD nCmdLen);
    /**
     * \brief 获取场景服务器的编号
     *
     * \return 场景服务器编号
     */
    const WORD getServerID() const
    {
      return wdServerID;
    }
  private:
    WORD wdServerID;

};

extern MiniClient *miniClient;

/**
 * \brief 时间回调函数
 */
class SceneTimeTick : public zThread
{

  public:

    /// 当前时间
    static zRTime currentTime;

    /**
     * \brief 析构函数
     */
    ~SceneTimeTick() {};

    /**
     * \brief 获取唯一实例
     */
    static SceneTimeTick &getInstance()
    {
      if (NULL == instance)
        instance = new SceneTimeTick();

      return *instance;
    }

    /**
     * \brief 释放类的唯一实例
     */
    static void delInstance()
    {
      SAFE_DELETE(instance);
    }

    void run();

  private:
	  /// sky 1秒钟记数器
	  Timer _one_sec;

    /// 五秒钟计数器
    Timer _five_sec;

    // 一分钟计数器
    Timer _one_min;

    // 竞赛进行标志
    bool quiz;

    /// 唯一实例
    static SceneTimeTick *instance;

    /**
     * \brief 构造函数
     */
    SceneTimeTick() : zThread("TimeTick"),_one_sec(1),_five_sec(5),_one_min(60),quiz(false) {};

};

/**
 * \brief 蒙面类
 *
 */
class Mask
{
public:
  enum {
    TIME_CONSUME_PERMANENCE = 10,
    ATTACKED_CONSUME_PERMANENCE = 1,
    DROP_ODDS = 720,
    ATTACK_USE_TIME = 10*1000,
  };
  
  Mask();
  ~Mask();

  int on_use(SceneUser* user,zObject* ob);
  
  int on_attack(SceneUser* victim);
  int on_defence();
  
  int on_timer();
  
  int on_trade();

  int on_gem();
  
  bool is_masking() const;

  bool is_use(zObject* ob) const;
      
private:
  int drop_odds() const;
  
  int reduce_permanence();

  zRTime _last;
  bool _drop;
  
  /// 蒙面物品
  zObject* _mask;
  /// 蒙面用户
  SceneUser* _user;
};

//荣誉模式需要的等级限制
#define TEAM_HONOR_MEMBER_LEVEL 50
#define TEAM_HONOR_LEADER_LEVEL 31

//sky ROLL选项
enum
{
	Roll_Null		= 0,			//无选择
	Roll_Exclude	= 1,			//离线或者离的太远被排除
	Roll_GiveUp		= 2,			//放弃
	Roll_Greed		= 3,			//贪婪
	Roll_Need		= 4				//需求
};

/**
 * \brief 组队成员结构
 */
struct TeamMember 
{
	DWORD id;
	DWORD tempid;
	char name[MAX_NAMESIZE + 1];
	DWORD offtime;
	DWORD begintime;
	BYTE rollitem;		//sky 保存该用户的ROLL选择项
	WORD rollnum;		//sky 保存ROLL所产生的点数

	/**
	* \brief 构造初始化结构
	*/
	TeamMember()
	{
		id = 0;
		tempid = 0;
		bzero(name,sizeof(name));
		offtime = 0;
		rollitem = Roll_Null;
		rollnum = 0;
	}
};

/**
 * \brief 队伍遍历方法
 */
struct TeamMemExec
{
  virtual ~TeamMemExec(){};
  virtual bool exec(TeamMember &member) = 0;
};

struct SceneUser;

struct Team
{
  /// 定义最大成员数
  static const  int MaxMember = MAX_TEAM_NUM;

  /**
   * \brief 构造函数初始化相关属性
   */
  Team()
  {
    leader = 0;
    averageExp = 0;
    current_obj_owner=0;

	//sky 新增队伍唯一ID
	dwTeam_tempid = 0;
  }

  /// 这个是队长的tempid 如果要改变请通知服务端全体成员
  DWORD leader;

  /// sky 队伍唯一ID
  DWORD dwTeam_tempid;

  //轮流物品掉落时当前掉落的序号
  DWORD current_obj_owner;

  /// 平均经验参数
  DWORD averageExp;

  const TeamMember *getTeamMember(DWORD index) const;
  const TeamMember *getTeamMember(const char * name) const;	//sky 重载用名字查询队员是否存在

  private:
  friend class TeamCondition;
  friend class TeamedCondition;
  friend class TeamAction;
  friend class QuestList;
  friend class SceneNpc;

  

  /// 队伍读写锁
  zRWLock rwlock;
  
  public:
	  /// 队伍成员表
	  std::vector<TeamMember> member;

	  /// 离线成员临时登记表
	  std::vector<TeamMember> offline_member;

	  /**
	  * \brief 获取队伍人数
	  */
	  int getSize()
	  {
		  return member.size();
	  }

	  /**
	  * \brief 获取该场景真实存在的人数
	  */
	  int getSceneSize()
	  {
		  int num = 0;
		  for(int i=0; i<member.size(); i++)
		  {
			  if(member[i].tempid != MEMBER_BEING_OFF)
				  num++;
		  }

		  return num;
	  }

	  /**
	  * \brief 清除所有的队员
	  */
	  void Clear()
	  {
		  member.clear();
	  }
	  DWORD getNextObjOwnerID();
	  void setNextObjOwnerID(DWORD id)
	  {
		  current_obj_owner=id;
	  }
	  void userupgrade(SceneUser *pUser);
	  void decreaseAverageExp(SceneUser *pUser);
	  void increaseAverageExp(SceneUser *pUser);
	  void calAverageExp(zPosI pos,DWORD sceneid);
	  bool addMember(DWORD id,DWORD tempid,const char *pname);
	  void removeMember(DWORD id,DWORD tempid);
	  void removeMemberByID(DWORD id);
	  DWORD removeMemberByName(const char *pname);
	  void removeMemberByTempID(DWORD tempid);
	  void execEvery(TeamMemExec &callback);
	  void execEveryExceptMe(TeamMemExec &callback,DWORD tempid);
	  void requestFriendDegree();
};

struct SceneUser;

/// 友好度成员
struct stDegreeMember{
  DWORD dwUserID;
  WORD  wdDegree;
  WORD  wdTime;
};

/// 友好度临时结构
struct stTempDegreeMember{
  DWORD dwUserID;
  WORD  wdDegree;
  WORD  wdTime;
  BYTE  byType;
};

/// 友好度范围 
const DWORD FRIENDDEGREE_RANGE_BTM = 3*60;

/// 友好度范围
const DWORD FRIENDDEGREE_RANGE_MED = 15*60;
// 友好度范围
const DWORD FRIENDDEGREE_RANGE_TOP = 55*60;

/// 夫妻防御增益百分比1
const DWORD FRIENDDEGREE_CONSORT_DEFPLUS_RATE_TOP  = 8;  
/// 夫妻防御增益百分比2
const DWORD FRIENDDEGREE_CONSORT_DEFPLUS_RATE_MED  = 5;  
/// 夫妻防御增益百分比3
const DWORD FRIENDDEGREE_CONSORT_DEFPLUS_RATE_BTM  = 3;  

/// 徒弟防御增益百分比1
const DWORD FRIENDDEGREE_PRENTICE_DEFPLUS_RATE_TOP  = 2;  
/// 徒弟防御增益百分比2
const DWORD FRIENDDEGREE_PRENTICE_DEFPLUS_RATE_MED  = 2;  
/// 徒弟防御增益百分比3
const DWORD FRIENDDEGREE_PRENTICE_DEFPLUS_RATE_BTM  = 2;  
/// 徒弟防御增益百分比100
const DWORD FRIENDDEGREE_PRENTICE_DEFPLUS_RATE_FULL    = 100;

/// 师傅攻击增益百分比1
const DWORD FRIENDDEGREE_TEACHER_ATTPLUS_RATE_TOP  = 4;  
/// 师傅攻击增益百分比2
const DWORD FRIENDDEGREE_TEACHER_ATTPLUS_RATE_MED  = 4;  
/// 师傅攻击增益百分比3
const DWORD FRIENDDEGREE_TEACHER_ATTPLUS_RATE_BTM  = 2;  

/// 好友攻击增益百分比1
const DWORD FRIENDDEGREE_FRIEND_ATTPLUS_RATE_TOP  = 8;  
/// 好友攻击增益百分比2
const DWORD FRIENDDEGREE_FRIEND_ATTPLUS_RATE_MED  = 5;  
/// 好友攻击增益百分比3
const DWORD FRIENDDEGREE_FRIEND_ATTPLUS_RATE_BTM  = 3;  

/// 经验增益百分比
const DWORD FRIENDDEGREE_EXPPLUS_RATE  = 5;  
/// 金钱增益百分比
const DWORD FRIENDDEGREE_MONEYPLUS_RATE = 20;  

/// 朋友关系增益概率1
const DWORD FRIENDDEGREE_FRIEND_RATE_TOP  = 100;  
/// 朋友关系增益概率2
const DWORD FRIENDDEGREE_FRIEND_RATE_MED  = 100;  
/// 朋友关系增益概率3
const DWORD FRIENDDEGREE_FRIEND_RATE_BTM  = 100;  

/// 夫妻关系增益概率1
const DWORD FRIENDDEGREE_CONSORT_RATE_TOP  = 100;  
/// 夫妻关系增益概率2
const DWORD FRIENDDEGREE_CONSORT_RATE_MED  = 100;  
/// 夫妻关系增益概率3
const DWORD FRIENDDEGREE_CONSORT_RATE_BTM  = 100;  

/// 师徒关系增益概率1
const DWORD FRIENDDEGREE_TEACHER_RATE_TOP  = 40;  
/// 师徒关系增益概率2
const DWORD FRIENDDEGREE_TEACHER_RATE_MED  = 20;  
/// 师徒关系增益概率3
const DWORD FRIENDDEGREE_TEACHER_RATE_BTM  = 20;  

/// 友好度增益有效距离
const DWORD FRIENDDEGREE_VALIDATION_DISTANCE_WIDE = SCREEN_WIDTH; 
/// 友好度增益有效距离
const DWORD FRIENDDEGREE_VALIDATION_DISTANCE_HEIGHT = SCREEN_HEIGHT; 

//sky ROLL物品结构
struct RollItemPos
{
	zPos ItemPos;	//sky 被Roll的物品
	DWORD SceneID;		//sky 物品所在的场景ID
};

/**
 * \brief  队伍管理器
 */
class TeamManager
{
  private:
    //物品分配模式
    BYTE obj_mode;
    //经验分配模式
    BYTE exp_mode;
    // /队伍对象
    Team team;

    /// 排序方法
    struct ltword
    {
      bool operator()(const WORD s1,const WORD s2) const
      {
        return s1>s2;
      }
    };

    /// 朋友列表
    std::map<WORD,struct stDegreeMember,ltword> friendList;

    /// 夫妻列表
    std::map<WORD,struct stDegreeMember,ltword> consortList;

    /// 师傅列表
    std::map<WORD,struct stDegreeMember,ltword> teacherList;

    /// 徒弟列表
    std::map<WORD,struct stDegreeMember,ltword> prenticeList;

    /// 社会关系读写锁
    zRWLock relationlock;

    /// 类型定义
    typedef std::map<WORD,struct stDegreeMember,ltword>::value_type insValueType;


    bool giveupstatus; //阵法放弃标志
    zRTime giveuptime; //阵法放弃时间

public:
	//sky ROLL定时器和标志
	DWORD dwRollTime;
	bool  bRoll;

	RollItemPos RollItem;

	//sky 设置Roll的物品
	void SetRollItem( zPos pos, DWORD SceneID )
	{
		RollItem.ItemPos.x = pos.x;
		RollItem.ItemPos.y = pos.y;
		RollItem.SceneID = SceneID;
	}

	//sky 返回Roll的物品位置
	RollItemPos * GetRollItem( )
	{
		return &RollItem;
	}

	//sky 清空ROLL相关标志(ROLL完毕用)
	void DelRoll()
	{
		zPos p;
		p.x = 0;
		p.y = 0;
		SetRollItem(p, 0);
		bRoll = false;
		dwRollTime = 0;
	}

	void setNextObjOwnerID(DWORD id)
	{
		team.setNextObjOwnerID(id);
	}
	DWORD getNextObjOwnerID()
	{
		return team.getNextObjOwnerID();
	}

	//检测是否是轮流拾取模式
	bool isSpecialObj()
	{
		return obj_mode == Cmd::TEAM_OBJ_MODE_SPECIAL;
	}
	//检测是否是普通拾取模式
	bool isNormalObj()
	{
		return obj_mode == Cmd::TEAM_OBJ_MODE_NORMAL;
	}
	//sky 新增加队长拾取模式
	bool isCaptainObj()
	{
		return obj_mode == Cmd::TEAM_OBJ_MODE_CAPTAIN;
	}

	BYTE getObjMode()
	{
		return obj_mode;
	}
	void setObjMode(BYTE mode)
	{
		obj_mode = mode;
	}
	bool isSpecialExp()
	{
		return exp_mode == Cmd::TEAM_EXP_MODE_SPECIAL;
	}
	bool isNormalExp()
	{
		return exp_mode == Cmd::TEAM_EXP_MODE_NORMAL;
	}
	BYTE  getExpMode()
	{
		return exp_mode;
	}
	void setExpMode(BYTE mode)
	{
		exp_mode = mode;
	}
	/**
	* \brief  构造初始化
	* \param  pUser 队伍管理器拥有者
	*/
	TeamManager()
	{
		giveupstatus = false;
		obj_mode=0;
		exp_mode=0;
		memset( &RollItem, 0, sizeof(RollItemPos) );
		dwRollTime =0;
		bRoll =false;
	}

	/**
	* \brief  判断队伍是否已经达到最大人数
	* \return true 队伍已经达到最大人数， false 还没有
	*/
	bool IsFull()
	{
		return team.getSize() == Team::MaxMember;
	}

	/**
	* \brief  获取队伍成员人数
	* \return 人数
	*/
	int getSize();

	int getExpSize(zPosI pos,DWORD sceneid);


	/**
	* \brief  是否组队
	* \return true 组队， false 没有组队
	*/
	bool IsTeamed()
	{
		return team.leader != 0;
	}

	/**
	* \brief  获取队长的临时id
	* \return 队长临时id
	*/
	DWORD getLeader() const
	{
		return team.leader;
	}

	/**
	* \brief  记录队长的临时id
	* \param  tempid 回调方法
	*/
	bool setLeader(DWORD tempid)
	{
		team.leader = tempid;

		return true;
	}

	/**
	* \brief  获取队伍的唯一id
	* \retrun 队伍唯一id
	*/
	DWORD getTeamtempId( )
	{
		return team.dwTeam_tempid;
	}

	/**
	* \brief  记录队伍的唯一id
	* \param  tempid 回调方法
	*/
	bool setTeamtempId(DWORD tempid)
	{		
		team.dwTeam_tempid = tempid;
		return true;
	}


	/**
	* \brief  角色升级数据变更处理
	* \param  pUser 升级的角色
	*/
	void userupgrade(SceneUser *pUser)
	{
		team.userupgrade(pUser);
	}

	/**
	* \brief  获得队伍平均分配经验
	* \return 平均经验
	*/
	DWORD getAverageExp()
	{
		return team.averageExp;
	}

	/**
	* \brief  获得队伍对象
	* \return 队伍对象
	*/
	Team& getTeam()
	{
		return team;
	}

	int getSceneSize()
	{
		return team.getSceneSize();
	}

	//sky 改变队长消息发送函数
	bool ChangeLeaderToSession(char * NewLeaberName = NULL);
	bool delMemberToSession(char * memberName);

	bool loadTeam(SceneUser *leader,TempArchiveMember *data);

	bool IsOurTeam(SceneUser *pUser);
	bool IsOurTeam(DWORD dwID);

	bool addNewMember(SceneUser *pUser,Cmd::stAnswerNameTeamUserCmd *rev, Cmd::Session::t_Team_AnswerTeam * rev1 = NULL);
	bool addNewMember(SceneUser *leader,SceneUser *pUser);

	//sky 重载一个不需要通知sess的添加队员函数(跨场景用)
	bool addNewMember(Cmd::Session::stMember Member, SceneUser *pUser=NULL); 

	//void removeTeam(SceneUser *pUser,DWORD tempid);
	void deleteTeam();

	void removeMember(Cmd::stRemoveTeamMemberUserCmd *rev);
	void kickoutMember(SceneUser *pUser,Cmd::stRemoveTeamMemberUserCmd *rev);
	void removeMember(SceneUser *mem);
	void checkOffline();
	void sendtoTeamCharData(SceneUser * leader,SceneUser *user);
	void sendtoTeamCharData(SceneUser *user);
	void sendCmdToTeam(SceneUser *user,void *cmd,DWORD cmdLen);
	void checkOffline(SceneUser *pUser);

	// SKY 修改改变队长的函数为不需要删除原来的队伍
	bool changeLeader(char * NewLeaberName = NULL);

	// SKY 通知所有队伍成员开始ROLL
	bool NoticeRoll(zObject * obj);

	// sky 返回队伍中在线的真实人数
	int GetTeamMemberNum();

	// sky 查询是不是所有的玩家都选择拉ROLL选项
	bool GetNoRollUser();

	// sky 设置队员的ROLL相关参数
	int SetMemberRoll( DWORD tempid, BYTE rolltype );

	// sky 全部队员选择ROLL完毕后开始计算ROLL最大的队员
	TeamMember * ComparisonRollnum();

	void execEveryOne(TeamMemExec &callback);
	void execEveryOneExceptMe(TeamMemExec &callback,DWORD tempid);
	void decreaseAverageExp(SceneUser *pUser);
	void calAverageExp(zPosI pos,DWORD sceneid);

	// 设置该用户的友好度相关社会关系
	void setFriendDegree(Cmd::Session::t_ReturnFriendDegree_SceneSession * = NULL);

	void countFriendDegree();
	void requestFriendDegree();
	bool canPutSkill();
	//sky 设置队员的存在状态
	bool SetMemberType(DWORD memberID, DWORD leaberID, bool Being);
	//sky 因为创建队伍现在要放到场景实现,所以把他现在归为公有访问
	bool addMemberByTempID(SceneUser *pUser,DWORD tempid);
	bool addMemberToSession(char * userName);
	bool addWAwayMember(Cmd::Session::stMember * AwayUser);
	void removeMemberByTempID(DWORD tempid);

	// sky 删除成员遍历通知所有队伍成员
	bool T_DelTeamExec(const Cmd::stRemoveTeamMemberUserCmd *rev);

	// sky 遍历成员通知更换队长的消息
	bool T_ChangeLeaderExec( const char * LeaderName);

	// sky Roll控制函数
	bool RollItem_A();

private:
	void sendFriendDegreeToSession();

	SceneUser *findNewLeader(SceneUser *pUser);
	bool addMemberByID(SceneUser * pUser,DWORD id);
	
	
	void removeMemberByID(DWORD id);
	
};

class TradeOrder
{
public:
  enum {
    WIDTH = 8,
    HEIGHT = 3
  };
  
  TradeOrder(SceneUser* owner);
  ~TradeOrder();

  SceneUser* target() const;

  void ready(SceneUser* target);
  bool canRequest();
  bool canAnswer();
  void begin();
  bool hasBegin();
  bool commit();
  void rollback();
  bool hasCommit();
  void finish();
  void cancel();    
  void reset();

  bool can_trade();
  void trade();
  
  void add(zObject* ob);
  void add_money(DWORD money);
  
  void remove(DWORD id);
  void clear();
      
  bool in_trade(zObject* ob) const;

private:
  
  std::map<DWORD,zObject*> _items;

  bool begined;
  bool commited;
  
  SceneUser * _me;
  SceneUser * _target;
  DWORD _targetid;

  time_t lastmove;
  DWORD _money;
};

class PrivateStore
{
public:
  enum {
    WIDTH = 6,
    HEIGHT = 7,
  };

  enum STEP {
    NONE = 0,
    START = 1,
    BEGIN = 2,
  } ;
  
  class SellInfo
  {
  public:
    SellInfo(zObject* ob=NULL,DWORD money=0,BYTE x=0,BYTE y=0) : _ob(ob),_money(money),_x(x),_y(y)
    { }

    zObject* object() {return _ob;}
    DWORD money() {return _money;}
    BYTE x() { return _x;}
    BYTE y() {return _y;}

  private:
    zObject* _ob;
    DWORD _money;
    BYTE _x;
    BYTE _y;
  };
    
  PrivateStore();
  ~PrivateStore();

  void step(STEP step_,SceneUser *pUser);
  STEP step();

  void add(zObject* ob,DWORD money,BYTE x,BYTE y);
  void remove(DWORD id);

  void show(SceneUser* target);

  SellInfo* sell_ob(DWORD id);
private:
  
  void clear();
  
  STEP _step;

  std::map<DWORD,SellInfo> _items;
};

class RepairCost : public PackageCallback
{
public:
  RepairCost() : _cost(0)
  { }

  bool exec(zObject* ob);
  
  
  DWORD cost() const
  {
    return _cost;
  }
  
private:
  DWORD _cost;
};

class RepairEquip : public PackageCallback
{
public:
  RepairEquip(SceneUser* user)  : _user(user)
  { }

  bool exec(zObject* ob);
  
private:
  SceneUser* _user;
  
};

class RepairEquipUseGold : public PackageCallback
{
public:
  RepairEquipUseGold(SceneUser* user)  : _user(user)
  { }

  bool exec(zObject* ob);
private:
  SceneUser* _user;
  
};

class userScriptTaskContainer;

enum taskType
{
	SPAMap = 0,//温泉地图
	end,
};


#include <zebra/ItemCoolTime.h>

//sky 用户战场数据
struct stUserBattfieldData
{
	DWORD BattfieldHonor;	//sky 战场荣誉
	DWORD SportsHonor;		//sky 竞技场荣誉点

	//sky 用户传进战场前的地图数据
	char MapName[MAX_NAMESIZE+1];	//sky 地图名
	int x;							//sky 坐标x
	int y;							//sky 坐标y

	stUserBattfieldData()
	{
		BattfieldHonor = 0;
		SportsHonor = 0;
		x = 0;
		y = 0;
		bzero(MapName, MAX_NAMESIZE+1);
	}
};

struct SceneUser:public SceneEntryPk
{

  public:


	  //para:  dupId,副本号.istemp,true从tempDups中删除,false,从circleDups中删除
	  //void delFromDups(unsigned short dupId,bool istemp);

	  //void clearDups(bool istemp)
	  //{
		//if(istemp)
		//	tempDups.clear();
		//circleDups.clear();
	 // }

	 // std::map<DWORD,unsigned short> tempDups;//临时性副本
	 // std::map<DWORD,unsigned short> circleDups;//周期性副本

	  bool userQuestEnterDup(DWORD mapId);

  private:

	userScriptTaskContainer *_userScriptTaskContainer;
    char replyText[MAX_CHATINFO];//自动回复

	bool m_bCanJump; // [ranqd] 玩家是否处于可跳转地图的状态

    //回收延时等到(毫秒)
    zRTime recycle_delay;

    static DWORD Five_Relation[];
    /*
       zRTime tenSecondTime;
       zRTime oneSecondTime;
       zRTime oneMinuteTime;
     */
    Timer _half_sec;
    Timer _one_sec;
    Timer _five_sec;
	Timer _3_sec;
    Timer _ten_sec;
    Timer _one_min;
    Timer _five_min;
    //存档定时器,存档间隙具有随机性,可以打乱存档频率
    RandTimer _writeback_timer;
    struct box_target
	{
		zObject *targetO;
		zObject *defaultO;
	    //DWORD default_id;
		//DWORD default_level;
		//DWORD default_kind;
		//DWORD target_id;
		//DWORD target_level;
		//DWORD target_kind;

		//DWORD default_item;
		//DWORD default_Level;
	};

    box_target box_item;

    ///5秒计数,每5秒+1
    DWORD _5_sec_count;

    /// 杀死怪物个数
    DWORD killedNpcNum;
    const zRTime loginTime;
    /// 在线时间统计
    DWORD lastIncTime;
    /// 上次叛国时间记录
    DWORD lastChangeCountryTime;
    /// 临时关闭保护
    DWORD temp_unsafety_state;
    /// 财产保护状态
    BYTE  safety;
    /// 财产保护设置
    BYTE safety_setup;

    /// 死亡等待时间,死亡后5分钟,系统自动使角色进行回城复活
    int deathWaitTime;

    //是否正在后退中(ms)
    int backOffing;

    //用户权限等级
    BYTE priv;

    // 交战记录
    std::vector<WarRecord> vWars;
    
    // 赠送材料数量（组）
  //  DWORD Give_MatarialNum;

    char wglog[MAX_CHATINFO];
    DWORD wglog_len;
    void getWgLog(DWORD);
  public:
    //must be deleted by Visitor
    
    //副本号索引
    //unsigned short dupIndex;

    ~SceneUser();
    void destroy();

	DWORD getID()
	{
		return this->id;
	}


	DWORD getTeamLeader()
	{
		TeamManager* team = SceneManager::getInstance().GetMapTeam(TeamThisID);

		if(team)
			return team->getLeader();
		else
			return 0;
 	}


	bool addObjToPackByThisID(DWORD thisid);

	bool addScriptTask(int _type,const char* _funcName,int _elapse,int p1);
	bool delScriptTask(int _type);




    // 参与家族运镖的玩家ID
    std::vector<SeptGuard> venterSeptGuard;
    // 参与家族运镖的玩家ID
    //std::vector<SeptGuard> vfinishSeptGuard;

    //提取队伍成员信息
	const TeamMember *getMember(WORD index);
	/*{	
		//TeamMember _member;
	    
		return team.getTeam().getTeamMember(index);
		//DWORD _size = members->size();
		//if(index < 0 || index > members->size() - 1)
		//	return false;
		//member.begintime = (*members)[index].begintime;
		//member.id = (*members)[index].id;
		//member.offtime = (*members)[index].id;
		//member.tempid = (*members)[index].tempid;
		//strcpy_s(member.name,(*members)[index].name);
		//return  &(*members)[index];
		//return true;
	    
	}*/

	WORD getTeamSize()
	{
		TeamManager* team = SceneManager::getInstance().GetMapTeam(TeamThisID);
		return team->getSize();
	}

  public:

    DWORD getLoginTime()
	{
		return (time(NULL) - loginTime.sec()) / 60;
	}

    // 赠送材料数量（组）
    DWORD Give_MatarialNum;
    
    //道具卡张数
    DWORD Card_num;

    Dice * miniGame;//正在玩的小游戏

    void petAutoRepair(zObject *);//宠物自动修装备

    DWORD lastKiller;//最后一个杀死自己的玩家的临时ID
    DWORD dropTime;//客户端举报外挂后,随机断线的时间

    enum
    {
      ACCPRIV_NEWBIE_EQUIP_AT_5_15    = 0x00000001, //5、15级时给予新手装备
      ACCPRIV_GOLD_EQUIP_AT_5_15      = 0x00000002, //5、15级时给予黄金装备
      ACCPRIV_GREEN_EQUIP_AT_5_25_50    = 0x00000004  //5、25、50级时给予绿色装备
    };

    void sendGiftEquip(WORD level);//送体验卡,黄金卡,绿色卡的装备
    void givePetPoint();//30,40,50,60级送宝宝修炼时间

    DWORD processCheckTime;//下次检测外挂的时间

    DWORD refreshPetPackSize();

    bool npcTradeGold(Cmd::stBuyObjectNpcTradeUserCmd *ptCmd,zObjectB *b,BYTE itemlevel);
    void autoReply(char *) const;

    DWORD adoptedCartoon;   //被收养的宠物ID
    std::map<DWORD,Cmd::t_CartoonData> cartoonList;
    CartoonPet * cartoon;//放出的卡通宠物
    std::map<DWORD,CartoonPet *> adoptList;//收养的卡通宠物

    typedef std::map<DWORD,Cmd::t_CartoonData>::iterator cartoon_it;
    typedef std::map<DWORD,CartoonPet *>::iterator adopt_it;

    //下次可以查询数据库的时间,拍卖用
    DWORD queryTime;

    bool isSendingMail;
    bool isGetingMailItem;

    /*      
     ** whj 
     ** 正在退出状态不再接受任何指令
     **/
    bool unReging;
#ifdef _TEST_DATA_LOG
    //测试数据统计
    CharTest chartest;
    void readCharTest(Cmd::Record::t_Read_CharTest_SceneRecord *rev);
    void writeCharTest(Cmd::Record::enumWriteBackTest_Type type);
#endif
    // PK模式
    BYTE pkMode;
    //系统设置
    union{
      struct{
        BYTE savePkMode;
        BYTE setting[19];
      };
      BYTE sysSetting[20];
    };
    DWORD chatColor[8];

    static const DWORD CALL_PET_REGION = 6;

    ///马
    Horse horse;
    ScenePet * ridepet;

    //在国外死亡复活到本国凤凰城,写死了^_^
    DWORD deathBackToMapID;

    //跨服跳地图在Session未返回时的临时记录
    char wait_gomap_name[MAX_NAMESIZE];

    void setDiplomatState(BYTE newstate);
    int isDiplomatState(); // 返回0为外交官状态,返回1为非外交官状态,返回2为是外交官但因为有采集手套,暂时无效

    void setCatcherState(BYTE newstate);
    bool isCatcherState() const; 
    // 交战记录操作
    typedef std::vector<WarRecord>::iterator WarIter;

    void addWarRecord(DWORD type,DWORD relationid,bool isAtt=false);
    void removeWarRecord(DWORD type,DWORD relationid=0);
    
    void clearWarRecord()
    {
      vWars.clear();
    }
    
    void setAntiAttState(DWORD type,DWORD relationid);
    //   type,对战状态,relationid对敌方社会关系ID
    bool isWarRecord(DWORD type,DWORD relationid);


    /// 判断是否在财产保护状态
    // 返回TRUE,表示指定的功能,正处在保护状态,返回FALSE表示指定的功能没在保护状态
    bool isSafety(BYTE byType);
    

    // 判断是否与某位玩家处于交战状态
    bool isWar(SceneUser* entry);

    // 判断玩家自己是否处于指定类型的交战状态
    bool isSpecWar(DWORD dwType);

    // 判断是否是攻方
    bool isAtt(DWORD dwType,DWORD relationid=0);

    // 判断是否允许反攻
    bool isAntiAtt(DWORD dwType,DWORD relationid=0);

    // 当前对战记录大小
    size_t warSize()
    {
      return vWars.size();
    }

    DWORD updateNotify; //hp,mp,sp 更新通知
    BYTE updateCount;  //更新记数
    
    // 答题状态
    bool isQuiz;      // 是否处于答题状态

    // 外交官状态
    bool isDiplomat;

    // 捕头状态
    bool isCatcher;


    // 判断是否与某位玩家处于交战状态

    //召唤宠物
	ScenePet *	summonPet(DWORD id,Cmd::petType type,DWORD standTime=0,DWORD sid=0,const char * petName = "",DWORD anpcid = 0,zPos pos=zPos(0,0),BYTE dir=4);
	//[sky]召唤幻影专用函数
	bool		MirageSummonPet(DWORD id,Cmd::petType type,DWORD standTime,WORD num,const char * petName = "",DWORD anpcid = 0,zPos pos=zPos(0,0),BYTE dir=4);
    //[sky] 召唤士兵(每个士兵都是一个独立的个体不同以召唤兽或者宠物)
	int	 summonSoldiers(DWORD id, Cmd::petType type, WORD num, DWORD sid=0, const char * name="", DWORD anpcid = 0,zPos pos=zPos(0,0),BYTE dir=4) {return NULL;}
    //SceneNpc * summonPet(DWORD id,DWORD standTime,DWORD anpcid = 0);
    bool killOnePet(ScenePet *);
    void killAllPets();
    void killSummon();
    void collectPets();
    void clearGuardNpc();

    Cmd::t_PetData petData;//弓手抓的宠物数据
    DWORD savePetState(BYTE *data);
    DWORD loadPetState(BYTE *data,int size);
    bool saveGuard;///是否保存镖车的临时档案
    bool saveAdopt;///是否保存
    DWORD saveTempPetState(BYTE *data,DWORD maxSize);
    DWORD loadTempPetState(BYTE *data);
	//sky 保存和读取冷却时间的临时档案
	DWORD saveItemCoolTimes(BYTE *data,DWORD maxSize);
	DWORD loadItemCoolTimes(BYTE *data);

    void saveCartoonState();

    DWORD dwChangeFaceID;        //易容后的NPC的类型ID

    enum {
      REST_RESTITUTE_SP = 3,//自动体力恢复
      WALK_RESTITUTE_SP = 1,//走路体力消耗
      RUN_CONSUME_SP = 1,//跑步体力消耗
    };
    WORD step_state; 

    DWORD accid;
    SceneTask *gatetask;
    CharBase charbase;
    CharState charstate;
    //const zCharacterB *charconst;
    WORD wdTire;
    WORD wdTirePer;
    bool sitdownRestitute;
    bool npcdareflag;
    DWORD npcdarePosX;
    DWORD npcdarePosY;
    DWORD npcdareCountryID;
    DWORD npcdareMapID;
    bool npcdareNotify;

    WORD  lastUseSkill; // 最后一次使用的技能
    bool  farAttack; // 箭灵箭侠专用

    //DWORD dwHorseID;
    DWORD dwBodyID;
    DWORD dwLeftHandID;
    DWORD dwRightHandID;
    DWORD dwBodyColorSystem;
    DWORD dwBodyColorCustom;

    char  unionName[MAX_NAMESIZE];          // 帮会名称
    char  septName[MAX_NAMESIZE];           // 家族名称
    char  caption[MAX_NAMESIZE];    // 国王或城主头衔
	char  armyName[MAX_NAMESIZE];    // 所在军队名称
	char  ShopAdv[MAX_SHOPADV];			//SHX 摆摊广告;


    bool  king; // 是否为国王
    bool  unionMaster; // 是否为帮主
    bool  septMaster;  // 是否为族长
    bool  emperor;  // 是否为皇帝
    BYTE  kingConsort; // 0不是国王和皇帝的配偶,1为国王的配偶,2为皇帝的配偶
    
    DWORD dwSeptRepute;  // 家族声望
    DWORD dwSeptLevel;  // 家族等级
    DWORD dwUnionActionPoint;  // 帮会行动力
    DWORD dwArmyState; // 在军队中的职位


    bool answerMarry; ///同意配偶结婚请求标志
    DWORD friendID;   ///结婚时未来配偶的ID

    //物品
    Packages packs;

    //药品
    Leechdom leechdom;

    QuestList quest_list;
    Mask mask;

    //组队
	/// sky 允许组队标志
	bool isOpen;
	/// sky 加入的队伍的唯一ID
	DWORD TeamThisID;
	//组队模式
    DWORD team_mode;

	/**
	* \brief  是否允许组队
	* \return true 允许组队 false 不允许组队
	*/
	bool IsOpen()
	{
		return isOpen;
	}

	/**
	* \brief  设置允许组队标志
	* \param  is 允许组队标志
	*/
	void setOpen(bool is)
	{
		isOpen = is;
	}

    //交易单
    TradeOrder tradeorder;
    PrivateStore privatestore;

    int messageOrder;          ///用户上一次收到的Message顺序
    zRTime lastCheckMessage;      ///最后一次检查Message时间
    DWORD  lastMoveTime;
    DWORD   moveFastCount;

    //访问Npc的信息
    DWORD npc_dwNpcDataID;        ///Npc数据编号
    DWORD npc_dwNpcTempID;        ///Npc临时编号

    ScenePet* guard; //护镖

    struct{
      DWORD dwMapID;
      DWORD dwPosX;
      DWORD dwPosY;
    } npcHoldData;    /// 家族控制的NPC相关数据

    //快捷键数据
    char accelData[1024];

    DWORD myOverMan;
    //找到自己的师父id,没有返回0
    DWORD getOverMan()
    {
      return myOverMan;
    }
    bool canVisitNpc(SceneNpc *pNpc);
    void visitNpc(const DWORD npcID,const DWORD npcTempID)
    {
      npc_dwNpcDataID = npcID;
      npc_dwNpcTempID = npcTempID;
    }

    bool checkVisitNpc(const DWORD npcID) const
    {
      return npcID == npc_dwNpcDataID;
    }

    void cancelVistNpc()
    {
      npc_dwNpcTempID = 0;
      npc_dwNpcDataID = 0;
    }

    SceneUser(const DWORD accid);

    void setHairType(const BYTE hairtype)
    {
      DWORD temp = hairtype;
      temp <<= 24;
      charbase.hair = (temp & HAIRTYPE_MASK) | (charbase.hair & HAIRRGB_MASK);
    }

    BYTE getHairType() const
    {
      return ((charbase.hair & HAIRTYPE_MASK) >> 24);
    }

    void setHairColor(const DWORD rgb)
    {
      charbase.hair = (rgb & HAIRRGB_MASK) | (charbase.hair & HAIRTYPE_MASK);
    }

    DWORD getHairColor() const
    {
      return (charbase.hair & HAIRRGB_MASK);
    }

    inline bool checkMessageTime(const zRTime &ct)
    {
      if (ct >= lastCheckMessage)
      {
        lastCheckMessage = ct;
        lastCheckMessage.addDelay(2000);
        return true;
      }
      else
        return false;
    }

    bool speedOutMove(DWORD time,WORD speed,int len);

    bool SceneUser::isNewCharBase() const
    {
      return !(CHARBASE_OK & charbase.bitmask);
    }
    bool IsPking()
    {
      return lastPkTime ? true : false;
    }
    bool getMagicType();
    int IsOppose(DWORD five);
    bool IsJoin(DWORD five);
    /*
       bool tenSecondTimer(const zRTime &ct);
       bool oneSecondTimer(const zRTime &ct);
       bool oneMinuteTimer(const zRTime &ct);
     */
    inline bool checkGoodnessTime(const zRTime &ct);
    void checkPunishTime();
    void reduceGoodness(SceneNpc *pNpc);
    void initCharBase(Scene *intoscene);

    void calReliveWeaknessProperty(bool enter);
    void enterSeptGuard();
    void finishSeptGuard();

    void setupCharBase(bool lock=true);
    bool upgrade(DWORD num = 0);
    void setStateToNine(WORD state);
    void clearStateToNine(WORD state);
    void sendGoodnessToNine();
    void sendMeToNine();
    void sendMeToNineDirect(const int direct);
    void sendNineToMe();
    void sendNineToMeDirect(const int direct);
	void SceneUser::sendDupChangeCmdToGate();
    void sendPetDataToNine();
    void sendInitToMe();
    void addNineSeptExp(DWORD dwSeptID);
    void addNineSeptNormalExp(DWORD dwSeptID);

    void checkAutoMP();
    bool checkUserCmd(const Cmd::stNullUserCmd *,const DWORD nCmdLen) const;
    bool checkChatCmd(const Cmd::stNullUserCmd *,const DWORD nCmdLen) const;
    void sendCmdToMe(const void *pstrCmd,const DWORD nCmdLen);
    void sendCmdToBill(const void *pstrCmd,const DWORD nCmdLen);
    void sendSceneCmdToBill(const void *pstrCmd,const DWORD nCmdLen);
    bool unreg(bool MoveScene = false);
    bool requestUser(Cmd::stRequestUserDataMapScreenUserCmd *rev);
    bool requestNpc(Cmd::stRequestMapNpcDataMapScreenUserCmd *rev);
    bool save(const Cmd::Record::WriteBack_Type writeback_type);
    bool attackMagic(const Cmd::stAttackMagicUserCmd *rev,const DWORD cmdLen);
    bool reliveReady(const Cmd::stOKReliveUserCmd *rev,bool isOrigin = false);
    bool relive(const int relive_type,const int delaytime,const int data_percent=100);
    //void Death();
    void setDeathState();
    void lostObject(SceneUser *pAtt = NULL);
    bool switchPKMode(const Cmd::stPKModeUserCmd *rev);
    bool checkGoodness();    
    char *getGoodnessName();
    short getGoodnessState() const;
    bool isPkAddition();
    DWORD getPkAddition();
    bool isRedNamed(bool allRedMode=true) const;
    float getGoodnessPrice(DWORD price,bool isBuy);
    bool move(Cmd::stUserMoveMoveUserCmd *rev);
    bool backOff(const int dir,const int grids);
    bool goTo(zPos &newPos);
    void jumpTo(zPos &newPos);
    bool goToRandomRect(zPos center,WORD rectx = SCREEN_WIDTH,WORD recty = SCREEN_HEIGHT);
    bool ride(Cmd::stRideMapScreenUserCmd *rev);
    bool useObject(zObject *obj, DWORD PreUseID=0);
    bool useLeechdom(zObject *obj);
    bool useScroll(zObject *obj);
    bool useCallObj(zObject *obj);
    bool useAmulet(zObject *obj);
    bool useSkill(zObject *obj);
    bool doPropertyCmd(const Cmd::stPropertyUserCmd *rev,DWORD cmdLen);
    bool doChatCmd(const Cmd::stChatUserCmd *rev,DWORD cmdLen);
    bool doTradeCmd(const Cmd::stTradeUserCmd *rev,DWORD cmdLen); 
    bool doGMCmd(char *gmcmd);
    bool doCountryCmd(const Cmd::stCountryUserCmd *rev,DWORD cmdLen);
    bool doArmyCmd(const Cmd::stArmyUserCmd *rev,DWORD cmdLen);
    bool doDareCmd(const Cmd::stDareUserCmd *rev,DWORD cmdLen);
    bool doNpcDareCmd(const Cmd::stDareUserCmd *rev,DWORD cmdLen);
    bool doUnionCmd(const Cmd::stUnionUserCmd *rev,DWORD cmdLen);
    bool doSafetyCmd(const Cmd::stSafetyUserCmd *rev,DWORD cmdLen);
    bool doSeptCmd(const Cmd::stSeptUserCmd *rev,DWORD cmdLen);
    bool doSchoolCmd(const Cmd::stSchoolUserCmd *rev,DWORD cmdLen);
    bool doRelationCmd(const Cmd::stRelationUserCmd *rev,DWORD cmdLen);
    bool doPetCmd(const Cmd::stPetUserCmd *rev,DWORD cmdLen);
    bool doQuizCmd(const Cmd::stQuizUserCmd *rev,DWORD cmdLen);
    bool doMailCmd(const Cmd::stMailUserCmd *rev,DWORD cmdLen);
    bool doAuctionCmd(const Cmd::stAuctionUserCmd *rev,DWORD cmdLen);
    bool doStockCmd(const Cmd::stStockSceneUserCmd *ptCmd,DWORD cmdLen);
    bool doPrisonCmd(const Cmd::stPrisonUserCmd *rev,DWORD cmdLen);
    bool doMiniGameCmd(const Cmd::stMiniGameUserCmd *rev,DWORD cmdLen);
    bool stockSave(DWORD mon,DWORD go);
    bool doCartoonCmd(const Cmd::stCartoonUserCmd *rev,DWORD cmdLen);
    void full_t_UserData(Cmd::t_UserData &data);
    void full_t_MapUserData(Cmd::t_MapUserData &data);
    void full_t_MapUserDataPos(Cmd::t_MapUserDataPos &data);
    void full_t_MapUserDataState(Cmd::t_MapUserDataState &data);
    void full_t_MapUserDataPosState(Cmd::t_MapUserDataPosState &data);
    void full_t_MainUserData(Cmd::t_MainUserData &data) const;
    void sendInitHPAndMp();
    bool addSkillData(const Cmd::stAddUserSkillPropertyUserCmd *rev);
    bool removeSkill(const Cmd::stRemoveUserSkillPropertyUserCmd *rev);
    bool upgradeSkill(DWORD dwSkillID,bool needSkillPoint=true);
    bool changeMap(Scene *newscene,const zPos &pos,bool ignoreUserLevel= false);
    bool intoScene(Scene *newscene,bool needInitInfo,const zPos &initPos);
    void setDeathBackToMapID(Scene *s);
    int saveSysSetting(BYTE*);
    int loadSysSetting(BYTE*);
    bool LeaveScene();
	bool userEnterDup(unsigned short _dupIndex,DWORD mapid,userDupMap *_userDupMap);
	void userLeaveDup();
    bool refreshMe();
    void sendExpToSept(const WORD &exp);
    void dreadProcess();				//sky 控制恐惧的胡乱移动
	void blindProcess();				//sky 控制失明的胡乱移动
    WORD getMyMoveSpeed() const;
    bool unCombin(Cmd::stUnCombinUserCmd *rev);
    void setNpcHoldData(Cmd::Session::t_notifyNpcHoldData *rev);
    inline void checkNpcHoldDataAndPutExp();
    void addObjectToUserPacket(int objectid,SceneUser *pUser);
    void catchMyPet();
    void removeNineEntry(zPosI posi);

    void addExp(DWORD,bool=true,DWORD dwTempID=0,BYTE byType=0,bool=false);
    void addPetExp(DWORD,bool=true,bool=false);

	//sky 检测玩家家的单前的战斗状态
	useFightState IsPveOrPvp();
	//sky 设置角色战斗状态
	void SetPveOrPvp(useFightState type); //设置pve或者Pvp状态
	//sky 设置角色战斗状态的时间
	int PkTime;					//sky 保存玩家进入战斗的时间
	void SetPkTime(DWORD time=10);		//sky 重新设置玩家进入战斗的时间
	bool IsPkTimeOver();			//sky 判断是否玩家能脱离战斗

	//sky 使用物品触发技能函数
	bool ItemUseSkill(zObject * obj, DWORD preUserID=0);

	//sky 新的物品冷却管理器
	CItmeCoolTime m_ItemCoolTimes;

	//sky 检测是否超过设定的随机范围
	virtual bool outOfRandRegion(zPos * pos);

	//sky 新增战场类数据对象
	stUserBattfieldData BattfieldData;
	DWORD saveBattfieldData(BYTE * data);
	DWORD loadBattfieldData(BYTE * data);

    //void petLevelUp(ScenePet *);
    /**
     * \brief 设置战斗类宠物的跟踪攻击目标
     */
    void setPetsChaseTarget(SceneEntryPk *entry);
	bool doBoxCmd(const Cmd::stCowBoxUserCmd *ptCmd,DWORD cmdLen);
	bool doTurnCmd(const Cmd::stTurnUserCmd *ptCmd,DWORD cmdLen);
	bool doReMakeObjCmd(const Cmd::stReMakUserCmd *ptCmd,DWORD cmdLen);

	// [sky] 训马指令处理函数
	bool doHorseTrainingCmd(const Cmd::stReMakUserCmd *ptCmd,DWORD cmdLen);

	//[sky] 装备自由加点处理函数
	bool doAddItemAttCmd(const Cmd::stAttruByteUserCmd *ptCmd,DWORD cmdLen);
	//[sky] 宝石镶嵌处理函数
	bool doMosaicGenCmd(const Cmd::stMakeObjectUserCmd *ptCmd,DWORD cmdLen);
	//[sky] 战场竞技场相关处理函数 begin
	bool doArenaCmd(const Cmd::stArenaUserCmd *ptCmd, DWORD cmdLen);
	//[sky] 战场竞技场相关处理函数 end
	zObject * GetObjectBydst( stObjectLocation * dst );			//根据格子位置获取格子里的物品指针
    /**
     * \brief 根据箭支的变化重算攻击属性
     * \calcflag 标志是否在函数内自行计算并通知
     * \return true为需要重算,false为不需要重算
     */
    //bool recalcBySword(bool calcflag = true);

    bool isAllied(SceneUser *pUser);

    /**
     * \brief 计算组队中各种社会关系的友好度
     */
    inline void countFriendDegree();

    /**
     * \brief 将客户端消息转发到会话服务器
     */
    bool forwardSession(const Cmd::stNullUserCmd *pNullCmd,const DWORD nCmdLen);

    void full_stRTMagicPosUserCmd(Cmd::stRTMagicPosUserCmd &ret) const
    {
      ret.pos.dwTempID=tempid;
      ret.pos.byDir=getDir();
      ret.pos.x=getPos().x;
      ret.pos.y=getPos().y;
      ret.byTarget=Cmd::MAPDATATYPE_USER;
    }

    /**
     * \brief 向包裹中添加物品
     *
     * \param srcObj 目标物品
     * \param needFind 需要查找位置
     * \param from_record 
     * \param calcflag
     *
     * \return 失败返回false,否则返回true
     */
    //bool packsaddObject(zObject *srcObj,bool needFind,bool from_record = false,bool calcflag = true);

    /**
     * \brief 根据物品id调整物品在包裹中的数量
     *
     * \param id 物品的objectid
     * \param num 减少的数量
     * \param upgrade 升级
     *
     *
     * \return 失败返回-1,否则返回0
     */
    int reduceObjectNum(DWORD id,DWORD num,BYTE upgrade = 0);

    /**
     * \brief 根据物品id调整物品在包裹中的数量,如果大于该物品最大数量则创建一个新物品
     *
     * \param id 物品的objectid
     * \param num 增加的数量
     * \param upgrade 升级
     * \param notify 通知
     * \param bindit 是否绑定
     *
     *
     * \return 失败返回-1,没有物品被创建返回0,包裹满返回1,成功添加所有创建的物品返回2
     */
    int addObjectNum(DWORD id,DWORD num,BYTE upgrade = 0,int notify = 0,bool bindit = false);


    /**
     * \brief 根据物品id调整物品在包裹中的数量,如果大于该物品最大数量则创建一个新物品（任务接口）加绿装绑定装备
     *
     * \param id 物品的objectid
     * \param num 增加的数量
     * \param upgrade 升级
     * \param notify 通知
     * \param bindit 是否绑定
     *
     *
     * \return 失败返回-1,没有物品被创建返回0,包裹满返回1,成功添加所有创建的物品返回2
     */
    int addGreenObjectNum(DWORD id,DWORD num,BYTE upgrade = 0,int notify = 0,bool bindit = true);


    /**
     * \brief 自动恢复用户相关属性,包括生命,魔法,体力等,该函数在UpdateSceneUserCallBack中被调用
     */
    inline void restitute();
    DWORD autoRestitute(DWORD &updated);

    /**
     * \brief 更新用户数据到会话
     */
    void updateUserData();

    /**
     * \brief 改变角色的hp
     *
     * \param hp 变更的HP
     *
     */
    void changeHP(const SDWORD &hp);

    /**
     * \brief 直接伤害
     *
     * \param pAtt 攻击者
     * \param dam 伤害值
     * \param notify 通知伤害显示
     */
    SWORD directDamage(SceneEntryPk *pAtt,const SDWORD &dam,bool notify=false);

    /**
     * \brief 改变角色的sp
     * \param sp 变更的SP
     */
    void changeSP(const SDWORD &sp);

    /**
     * \brief 改变角色的mp
     * \param mp 变更的MP
     */
    void changeMP(const SDWORD &mp);

    /**
     * \brief 获得最大的hp
     * \return 返回最大值
     */
    DWORD getMaxHP();

    /**
     * \brief 获得最大的hp
     * \return 返回最大值
     */
    DWORD getBaseMaxHP();

    /**
     * \brief 获得最大的mp
     * \return 返回最大值
     */
    DWORD getMaxMP();

    /**
     * \brief 获得最大的mp
     * \return 返回最大值
     */
    DWORD getBaseMaxMP();
    /**
     * \brief 重算并通知
     */
    void changeAndRefreshHMS(bool lock=true,bool sendData=true);
    /**
     * \brief 获得当前魔法攻击力
     * \return 魔法攻击力
     */
    virtual DWORD getMaxMDamage();

    /**
     * \brief 获得当前物理攻击力
     * \return 物理攻击力
     */
    virtual DWORD getMaxPDamage();

    /**
     * \brief 获得当前物理防御力
     * \return 物理防御力
     */
    virtual DWORD getPDefence();

    /**
     * \brief 获得当前魔法防御力
     * \return 魔法防御力
     */
    virtual DWORD getMDefence();

    /**
     * \brief 在自己被攻击之前的处理,包括,装备损耗处理,攻击有效几率判断等
     * \param pUser 攻击者
     * \param rev 本次攻击的触发指令
     * \param physics
     * \param good
     * \return true为这次攻击是有效的,false为一次无效的攻击
     */
    bool preAttackMe(SceneEntryPk *pUser,const Cmd::stAttackMagicUserCmd *rev,bool physics=true,const bool good = false);

    /**
     * \brief 角色被攻击
     * \param pEntry 攻击者
     * \param rev 本次攻击的触发消息
     * \return true为这次攻击是有效的,false为一次无效的攻击
     */
    //    virtual bool AttackMe(SceneEntryPk *pEntry,const Cmd::stAttackMagicUserCmd *rev,bool physics=true);

    /**
     * \brief 让角色重生
     */
    virtual void relive();

    /**
     * \brief 角色被击退N格
     * \param dwAttTempID 攻击者的临时ID
     * \param grids
     */
    void standBack(const DWORD dwAttTempID,DWORD grids);

    /**
     * \brief 读取临时存档数据
     * \param revData 得到的临时数据
     * \param dwSize 临时数据的大小
     */
    void setupTempArchive(const char *revData,const DWORD dwSize);

    DWORD setupBinaryArchive(const char *revData);
    bool needSaveBinayArchive(BinaryArchiveType type);
    DWORD addBinaryArchiveMember(DWORD type,char *out,DWORD maxSize);
    DWORD saveBinaryArchive(BYTE *out,const int maxsize);
    /**
     * \brief 让角色死亡
     */
    void toDie(const DWORD &dwTempID);

    /**
     * \brief 屏幕内随机移动
     */
    void goToRandomScreen();

    /**
     * \brief 通知客户端生命值的变化
     */
    void attackRTHpAndMp();

    /**
     * \brief 判断角色是否死亡
     * \return true为死亡
     */
    bool isDie();

    /**
     * \brief 获取角色的级别
     */
    DWORD getLevel() const;

    /**
     * \brief 获取角色的五行类型
     * \return 五行类型
     */
    DWORD getFiveType() const;

    /**
     * \brief 获取角色的五行点数
     * \return 五行点数
     */
    DWORD getFivePoint() const;

    /**
     * \brief 获取角色的五行级别
     * \return 五行级别
     */
    DWORD getFiveLevel() const;

    /**
     * \brief 需要的职业类型,决定可以使用的技能类型
     */
    bool addSkillToMe(zSkill *skill);

    /**
     * \brief 需要的职业类型,决定可以使用的技能类型
     */
    bool needType(const DWORD &needtype);

    /**
     * \brief 是否有该技能需要的武器
     * \return true 有 false 没有
     */
    bool needWeapon(DWORD skillid);

    /**
     * \brief 获取当前手持的武器类型
     * \return 武器类型
     */
    BYTE getWeaponType();

    /**
     * \brief 是否Pk区域
     * \param other 相关人
     * \return true 是 false 否
     */
    bool isPkZone(SceneEntryPk *other=NULL);

    /**
     * \brief 依赖物品消耗型法术
     * \param object 消耗物品的类型
     * \param num 消耗物品的数量
     * \return true 消耗成功 false 失败
     */
    bool reduce(const DWORD &object,const BYTE num);

    bool checkReduce(const DWORD &object,const BYTE num);

    /**
     * \brief 施放技能所导致的消耗MP,HP,SP
     * \param base 技能基本属性对象
     * \return true 消耗成功 false 失败
     */
    bool doSkillCost(const zSkillB *base);

    /**
     * \brief 检查施放技能所导致的消耗MP,HP,SP是否足够
     * \param base 技能基本属性对象
     * \return true 消耗成功 false 失败
     */
    bool checkSkillCost(const zSkillB *base);

    /**
     * \brief 检查自身的施放成功几率,决定这次技能是否可以施放
     * \return true 成功 false 失败
     */
    bool checkPercent();

    /**
     * \brief 判断是否是敌人
     * \return true 是 false 不是
     */
    int isEnemy(SceneEntryPk *entry,bool notify = false,bool good = false);

    /**
     * \brief 获取自己的主人,一般针对NPC而言,Player的主人是自己
     * \return NULL或者主人的对象指针
     */
    SceneEntryPk *getMaster();
    /**
     * \brief 得到最上层的主人
     * 返回自己
     * \return  自己
     */
    SceneEntryPk *getTopMaster();// const;

    /**
     * \brief 检查是否在禁言期
     * 
     * \return 如果在禁言期返回true,否则false
     */ 
    bool isForbidTalk()
    {
      zRTime ctv;
      return ctv.sec() < charbase.forbidtalk;
    }
    /** 
     * \brief 设置禁言时间
     * \param delay 延迟的秒数
     * \return 禁言到期的时间()
     */
    zRTime delayForbidTalk(int delay)
    {
      zRTime ctv(delay*1000);
      charbase.forbidtalk = ctv.sec();
      return ctv;
    }

    /**
     * \brief 设置玩家的权限
     * \param lv 等级
     */
    void setPriv(BYTE lv)
    {
      priv = lv;
    }

    /**
     * \brief 得到玩家的权限等级
     * \return 玩家的权限等级
     */
    BYTE getPriv()
    {
      return priv;
    }

    /**
     * \brief 重新发送本角色的地图数据
     */
    void reSendMyMapData();

    /**
     * \brief 通知选中自己的用户的hp和mp发生变化
     */
    void sendtoSelectedHpAndMp();
    void sendtoSelectedPkAdditionState();
    void sendtoSelectedPkAdditionStateToUser(SceneUser *pUser);
    void sendtoSelectedReliveWeakStateToUser(SceneUser *pUser);
    void sendtoSelectedReliveWeakState();
    void sendtoSelectedTrainStateToUser(SceneUser *pUser);
    void sendtoSelectedTrainState();


    /**
     * \brief 获取抗毒增加  
     */
    SWORD getPoisondef();

    /**
     * \brief 获取抗麻痹增加        
     */
    SWORD getLulldef();

    /**
     * \brief 获取抗眩晕增加        
     */
    SWORD getReeldef();

    /**
     * \brief 获取抗噬魔增加        
     */
    SWORD getEvildef();

    /**
     * \brief 获取抗噬力增加        
     */
    SWORD getBitedef();

    /**
     * \brief 获取抗混乱增加        
     */
    SWORD getChaosdef();

    /**
     * \brief 获取抗冰冻增加        
     */
    SWORD getColddef();

    /**
     * \brief 获取抗石化增加        
     */
    SWORD getPetrifydef();

    /**
     * \brief 获取抗失明增加        
     */
    SWORD getBlinddef();

    /**
     * \brief 获取抗定身增加        
     */
    SWORD getStabledef();

    /**
     * \brief 获取抗减速增加        
     */
    SWORD getSlowdef();

    /**
     * \brief 获取抗诱惑增加
     */
    SWORD getLuredef();


    /**
     * \brief 获取毒增加  
     */
    SWORD getPoison();

    /**
     * \brief 获取麻痹增加        
     */
    SWORD getLull();

    /**
     * \brief 获取眩晕增加        
     */
    SWORD getReel();

    /**
     * \brief 获取噬魔增加        
     */
    SWORD getEvil();

    /**
     * \brief 获取噬力增加        
     */
    SWORD getBite();

    /**
     * \brief 获取混乱增加        
     */
    SWORD getChaos();

    /**
     * \brief 获取冰冻增加        
     */
    SWORD getCold();

    /**
     * \brief 获取石化增加        
     */
    SWORD getPetrify();

    /**
     * \brief 获取失明增加        
     */
    SWORD getBlind();

    /**
     * \brief 获取定身增加        
     */
    SWORD getStable();

    /**
     * \brief 获取减速增加        
     */
    SWORD getSlow();

    /**
     * \brief 获取诱惑增加
     */
    SWORD getLure();



    /**
     * \brief 发送选中玩家的状态
     * \param state 状态
     * \param value 值
     * \param time 持续时间
     * */
    void sendtoSelectedState(DWORD state,WORD value,WORD time);
    /**
     * \brief 发送玩家的状态给自己
     * \param state 状态
     * \param value 值
     * \param time 持续时间
     * */
    void sendSevenStateToMe(DWORD state,WORD value,WORD time);

    /**
     * \brief 获取装备伤害加成
     * \return 伤害加成
     */
    WORD getDamageBonus();

    /**
     * \brief 设置战斗类宠物的跟踪攻击目标
     * \param weaponType 武器类型
     * \return true武器类型符合,false武器类型不符合
     */
    bool checkWeapon(BYTE weaponType);

    /*
    ///脱离战斗状态的时间
    zRTime endBattleTime;
    ///当前的对手
    SceneEntryPk * curTarget;

    void setEndBattleTime(const zRTime &,int);
    bool checkEndBattleTime(const zRTime &);
    void leaveBattle();
     */


    // 脱离队伍
    void leaveTeam();

    /**
     * \brief 判断是否可以回收用户内存
     * \param cur: 当前时间
     * \return 是否可以回收内存
     * */
    bool canRecycle(const zRTime &cur)
    {
      return recycle_delay <= cur;
    }

    /**
     * \brief 易容处理
     * \param cmd 易容消息
     * \param cmdLen 消息长度
     * \return true 处理成功 false 失败
     **/
    bool changeFace(const Cmd::stChangeFaceMapScreenUserCmd *cmd,const DWORD cmdLen);

    /**
     * \brief 武器提升对应技能等级
     * \param skilltype 技能的类型
     * \param skillkind 技能的系别
     * \return 提升的技能登记数
     **/
    WORD skillUpLevel(WORD skilltype,WORD skillkind);

    /**
     * \brief 检查是否骑马
     * \return true 骑马 false 没骑
     */
    bool checkMountHorse();
    inline void initTire();
    void  initAnswerCount();
    inline void processTire();
    void callProcessTire();
    inline void processAnswerCount();
    void sitdown();
    void standup();
    bool isSitdown();
    void getSummonAppendDamage(WORD &minDamage,WORD &maxDamage);
    void exchangeMeAndSummonPet();
    DWORD getMP();
    void clearMana();
    SceneEntryPk* getSceneEntryPk(DWORD entryType,DWORD entryID) const;
    void appendAttack(const Cmd::stAttackMagicUserCmd *rev);
    bool captureIt(SceneNpc *,BYTE);
    void leech(DWORD dwDam);
    void checkNpcDare(Cmd::Session::t_NpcDare_NotifyScene_SceneSession * rev);
    void checkNpcDareState();
    void notifySessionNpcDareResult();
    void npcDareGetGold(DWORD dwGold);
    void sendMessageToMe(const char *pattern);
    void reflectSkill(SceneEntryPk *pAtt,const Cmd::stAttackMagicUserCmd *rev);
    SWORD getWeaponPower(int powerkind);
    void relivePet();
    void sendSkill(WORD wdSkillID,BYTE level=1,DWORD target=0,BYTE attackType=Cmd::ATTACKTYPE_U2U,BYTE action=Cmd::Ani_Null);
    /**
     * \brief 设置角色的当前状态,并根据当前状态呈现角色的特效或者关闭特效
     * \param state 状态ID 根据enum SceneEntryState取值
     * \return true为这次攻击是有效的,false为一次无效的攻击
     */
    void showCurrentEffect(const WORD &state,bool isShow,bool notify=true);

  private:
    /**
     * \brief 处于蒙面状态时攻击别人发出系统提示
     * \param entry 攻击对象
     * */
    void processMaskOnAttack(SceneEntryPk *entry);
    /**
     * \brief 处于蒙面状态时被攻击
     * */
    void processMaskOnDefence();
    /**
     * \brief 计算伤害累加值
     * 
     * \param dwDam 输出,伤害值
     * \param dwDamDef 防御力
     * \param physics 是否物理攻击
     * \return
     * */
    void processAddDam(int &dwDam,int &dwDamDef,bool physics);
    /**
     * \brief 计算减少伤害
     * \param dwDam 伤害值
     * \param dwDamDef 防御力
     * \param 是否物理攻击
     * 
     * */
    void reduceDam(int &dwDam,int &dwDamDef,bool physics);
    /**
     * \brief 计算伤害反弹
     * \param dwDam 伤害值
     * \param dwDamDef 防御力
     * \param 是否物理攻击
     * */
    void reflectDam(int &dwDamDef,int &dwDamSelf,DWORD skillID);
    /**
     * \brief 处理玩家死亡
     * */
    bool processDeath(SceneEntryPk *pAtt);
    /**
     * \brief hp到mp转换
     * \param dwDamDef 防御力
     * 
     * */
    void hp2mp(int &dwDamDef);

    /**
     * \brief 计算预处理值
     * 
     * */
    void calPreValue();
    /**
     * \brief 设置魅力值
     * 
     * */
    void setupCharm();
    /**
     * \brief 保存玩家临时数据
     * 
     * */
    void saveTempArchive();
    /**
     * \brief 存储玩家临时文档
     * \param type 临时数据类型
     * \param out 保存数据开始的位置
     * \param maxSize 最大容量
     * \return 增加的字节
     * */
    DWORD addTempArchiveMember(DWORD type,char *out,DWORD maxSize);
    /**
     * \brief 检查某类型数据是否需要保存到临时文档
     * \param 要保存的数据类型
     * \return 是否需要保存
     * */
    bool needSaveTempArchive(TempArchiveType type);

    zRTime& waitRecycle();
    void notifyEquipChange();

    bool do_trade_rs_cmd(const Cmd::stTradeUserCmd *rev,DWORD cmdLen);

        bool SceneUser::processBackdoor(Cmd::stChannelChatUserCmd *pCmd,DWORD cmdLen);


	//Shx Add 装备中套装附加的属性;
	stIncrementBySuit Increment;
	/**
	* \ 计算装备中套装附加的属性
	* \param type 临时数据类型
	* \param out 保存数据开始的位置
	* \param maxSize 最大容量
	* \return 增加的字节
	* */
	void GetIncrementBySuit();
	//End shx
};


extern void sendMail(char * fromName,DWORD fromID,char * toName,DWORD toID,BYTE type,DWORD money,zObject * o,char * text);

class SceneUserManager:protected zUserManager
{
  protected:
    inline bool getUniqeID(DWORD& tempid);
    inline void putUniqeID(const DWORD& tempid);
    SceneUserManager();
    virtual ~SceneUserManager();
    static SceneUserManager *sum;
  public:
    static SceneUserManager &getMe();
    static void destroyMe();
    SceneUser * getUserByName( const char * name);
    SceneUser * getUserByID( DWORD id);
    SceneUser * getUserByTempID( DWORD tempid);
    SceneUser * getUserByNameOut( const char * name);
    SceneUser * getUserByIDOut( DWORD id);
    SceneUser * getUserByTempIDOut( DWORD tempid);
    bool addUser(SceneUser *user);
    void removeUser(SceneUser *user);
    void removeUserInOneScene(Scene *scene);
    void removeUserToHuangcheng(Scene *scene);
    void clearEmperorDare(Scene* scene);
    void setEmperorDare(Scene* scene);
    void removeUserByTask(SceneTask *task);
    void removeAllUser();
    DWORD countUserInOneScene(Scene *scene);
    DWORD countUserByTask(SceneTask *task);
    void countryTrans(DWORD dwCountryID,DWORD dwLevel);
    void setAntiAtt(DWORD dwType,DWORD dwFromRelationID,DWORD dwToRelationID);
    void enterWar(Cmd::Session::t_enterWar_SceneSession* cmd);
};


class SceneRecycleUserManager: public zEntryManager< zEntryID > 
{
  private:
    /**
     ** \brief 管理器访问互斥锁
     **/
    zRWLock rwlock;
    static SceneRecycleUserManager *instance;
    SceneRecycleUserManager(){}
    ~SceneRecycleUserManager(){}
  public:
    static SceneRecycleUserManager &getInstance();
    static void destroyInstance();
    SceneUser* getUserByID(DWORD id);
    void removeUser(SceneUser *user);
    bool addUser(zSceneEntry *user);
    bool canReg(DWORD id);
    void refresh();
    /**
     ** \brief 对每个用户执行
     ** \param exec 执行接口
     **/
    template <class YourUserEntry>
      bool execEveryUser(execEntry<YourUserEntry> &exec)
      {
        rwlock.rdlock();
        bool ret=execEveryEntry<>(exec);
        rwlock.unlock();
        return ret;
      }

};

/**
 * \brief AI类型,npc会做的基本动作
 * 走、攻击、巡逻、跳转等
 *
 */
enum SceneNpcAIType
{
  NPC_AI_NORMAL,///普通AI
  NPC_AI_SAY,///说话
  NPC_AI_MOVETO,///移动到某位置
  NPC_AI_ATTACK,///在某范围内攻击
  NPC_AI_FLEE,///逃离玩家
  NPC_AI_RETURN_TO_REGION,///回到活动范围
  NPC_AI_GO_ON_PATH,///按照一定路线移动
  NPC_AI_CHANGE_MAP,///切换地图（同一服务器内）
  NPC_AI_WARP,///同一地图内瞬间移动
  NPC_AI_PATROL,///巡逻
  NPC_AI_CLEAR,///清除该npc
  NPC_AI_WAIT,///等待,什么也不做
  NPC_AI_GUARD_DOOR,///门卫
  NPC_AI_GUARD_ARCHER,///弓卫
  NPC_AI_GUARD_GUARD,///侍卫
  NPC_AI_GUARD_PATROL,///巡逻卫士
  NPC_AI_DROP_ITEM,///丢东西
  NPC_AI_CALL_PET,///招宠物
  NPC_AI_RANDOM_CHAT///随机说话
};

///sky npc行为模式
enum NpcAIFlag
{
	AIF_ATK_PDEF            = 0x00000001, ///优先攻击物防最低的敌人
	AIF_ATK_MDEF            = 0x00000002, ///优先攻击魔防最低的敌人
	AIF_ATK_HP              = 0x00000004, ///优先攻击生命值最低的敌人
	AIF_GIVEUP_10_SEC       = 0x00000008, ///追逐10秒放弃目标
	AIF_GIVEUP_6_SEC        = 0x00000010, ///6秒未受到伤害放弃目标
	AIF_GIVEUP_3_SEC        = 0x00000020, ///3秒未受到伤害放弃目标
	AIF_FLEE_30_HP    = 0x00000040, ///HP30%以下逃跑4格
	AIF_FLEE_3_ENEMY_4  = 0x00000080, ///被3个以上敌人围攻逃跑4格
	AIF_NO_BATTLE    = 0x00000100,///非战斗npc
	AIF_NO_MOVE    = 0x00000200, ///不移动（弓卫、买卖、路标等）
	AIF_WARP_MOVE    = 0x00000400, ///瞬移方式移动
	AIF_CALL_FELLOW_7       = 0x00000800, ///召唤7*7范围的未激活npc(几率50%)
	AIF_CALL_FELLOW_9       = 0x00001000, ///召唤9*9范围的未激活npc（几率50%）
	AIF_CALL_BY_ATYPE       = 0x00002000, ///召唤同种攻击类型的同伴（与上两个标志合作）
	AIF_HELP_FELLOW_5  = 0x00004000,///帮助5*5范围内的同伴攻击（用于被动npc）
	AIF_ATK_MASTER    = 0x00008000,///直接攻击宠物的主人
	AIF_ATK_REDNAME    = 0x00010000,///攻击红名的玩家
	AIF_DOUBLE_REGION  = 0x00020000,///搜索范围加倍
	AIF_SPD_UP_HP20    = 0x00040000,///hp20%以下移动速度加倍
	AIF_ASPD_UP_HP50  = 0x00080000,///hp50%以下攻击速度加倍
	AIF_ACTIVE_MODE    = 0x00100000,///主动攻击
	AIF_RUN_AWAY    = 0x00200000,///逃离敌人
	AIF_LOCK_TARGET    = 0x00400000,///不切换攻击目标直到死
	AIF_RCV_UNDER_30  = 0x00800000,///hp30%以下持续回血1%
	AIF_RCV_REST    = 0x01000000,///脱离战斗30秒后回血一次5%
	AIF_LIMIT_REGION  = 0x02000000  ///限制活动范围
};

///npc说话的类型
enum NpcChatType
{
  NPC_CHAT_ON_FIND_ENEMY = 1,///发现敌人
  NPC_CHAT_ON_ATTACK,///攻击时说的话
  NPC_CHAT_ON_RETURN,///追逐返回时说的话
  NPC_CHAT_ON_DIE,///死的时候说的话
  NPC_CHAT_ON_FLEE,///逃跑时说的话
  NPC_CHAT_ON_HIT,///被打时说的话
  NPC_CHAT_ON_HELP,///帮助同伴时说的话
  NPC_CHAT_ON_BE_HELP,///同伴来帮助时说的话
  NPC_CHAT_RANDOM    ///随机说话
};

// 怪物使用技能的条件 [sky add 先做这些 后期再扩充]
enum NpcSkillCond
{
	NPC_COND_ALWAYS,			//无条件
	NPC_COND_HPGMAXRATE,		//自身HP高于n%时
	NPC_COND_HPLMAXRATE,		//自身HP低于n%时
	NPC_COND_FHPLMAXRATE,		//友方魔物HP低于n%时。范围是周围8格半径
	NPC_COND_ATPCGE,			//攻击的PC数量>=n的话
	NPC_COND_TARGEHPGMAX,		//攻击对象的HP高于n%时
	NPC_COND_TARGEHPLMAX,		//攻击对象的HP低于n%时
	NPC_COND_TARGETSTATUON,		//当攻击对象得到某个异常状态时(以cond2为基准)
	NPC_COND_TARGETSTATUOFF,	//当攻击对象没有某个异常状态时(以cond2为基准)
	NPC_COND_MAXSTHPGMAX,		//主人的HP高于n时(预留给智能BB用)
	NPC_COND_MAXSTHPLMAX,		//主人的HP低于n时(预留给智能BB用)
	NPC_COND_MASTERSTATUON,		//当主人没有某个异常状态时(以cond2为基准)(预留给智能BB用)
	NPC_COND_MASTERSTATUOFF,	//当主人得到某个异常状态时(以cond2为基准)(预留给智能BB用)
	NPC_COND_CLOSEDATACKED,		//被近身攻击时
	NPC_COND_LONGRANGEATTACKED,	//被远程攻击时
	NPC_COND_RUDEATTACKED,		//被攻击而不能反击时(暂时用不到)

	//异常状态的状况用于 NPC_COND_TARGETSTATUON  NPC_COND_TARGETSTATUOFF 等（暂时只留拉2个接口）
	NPC_COND_STAN,		//昏眩
	NPC_COND_SLEEP		//睡眠
};
/**
 * \brief 一个AI的定义
 *
 */
struct t_NpcAIDefine
{
  ///类型,NPC在该阶段的主要动作
  SceneNpcAIType type;
  ///位置 根据不同动作位置的意义也略不相同
  ///移动时表示目的地,其他表示活动范围中心
  zPos pos;
  ///范围 
  ///移动时表示到达目的地的判定范围,其他表示活动范围
  int regionX,regionY;
  //zRTime endTime;
  ///该AI的持续时间
  int lasttime;

  ///是否正在逃跑
  bool flee;
  ///逃跑的方向
  int fleeDir;
  ///逃跑计数
  int fleeCount;

  ///切换地图时,要去的地图
  ///说话时,要说的话
  char str[MAX_CHATINFO];


  /**
   * \brief 默认构造函数
   *
   */
  t_NpcAIDefine()
    :type(NPC_AI_NORMAL),pos(zPos(0,0)),regionX(2),regionY(2),lasttime(0),flee(false),fleeDir(-1),fleeCount(0)
    {
      bzero(str,sizeof(str));
    }
  
  /**
   * \brief 构造函数
   *
   *
   * \param type AI类型
   * \param pos 位置
   * \param regionX 范围宽
   * \param regionY 范围高
   * \param lasttime 持续时间
   * \return 
   */
  t_NpcAIDefine(SceneNpcAIType type,const zPos &pos,int regionX,int regionY,int lasttime)
    :type(type),pos(pos),regionX(regionX),regionY(regionY),lasttime(lasttime)
    {
      bzero(str,sizeof(str));
    }


  /**
   * \brief 拷贝构造函数
   *
   * \param ad 要复制的对象
   */
  t_NpcAIDefine(const t_NpcAIDefine &ad)
  {
    type = ad.type;
    pos = ad.pos;
    regionX = ad.regionX;
    regionY = ad.regionY;
    lasttime = ad.lasttime;
    flee = ad.flee;
    fleeDir = ad.fleeDir;
    fleeCount = ad.fleeCount;
    strncpy_s(str,ad.str,sizeof(str)-1);
  }

  /**
   * \brief 赋值
   *
   * \param ad 要拷贝的对象
   * \return 返回自身地址
   */
  t_NpcAIDefine & operator = (const t_NpcAIDefine &ad)
  {
    type = ad.type;
    pos = ad.pos;
    regionX = ad.regionX;
    regionY = ad.regionY;
    lasttime = ad.lasttime;
    flee = ad.flee;
    fleeDir = ad.fleeDir;
    fleeCount = ad.fleeCount;
    strncpy_s(str,ad.str,sizeof(str)-1);

    return *this;
  }
};


/**
 * \brief AI控制器
 * 控制器可以读取NPC脚本,使NPC按照脚本来动作
 * 
 * 控制器处理各种事件,时间、被攻击、死亡等
 * 根据不同的条件为NPC设置不同的AI
 *
 */
class NpcAIController
{
  static const int npc_call_fellow_rate;///NPC召唤同伴的几率
  static const int npc_one_checkpoint_time;///NPC按照路线移动时,走一个路点的最长时间
  static const int npc_checkpoint_region;///NPC移动,到达一个路点的判定范围
  static const int npc_onhit_stop_time;///任务NPC移动中被攻击时,停止的时间
  static const int npc_flee_distance;///NPC逃离攻击者的距离
  static const int npc_min_act_region;///NPC最小活动范围
  
  ///AI容器,AI按照在容器中的顺序依次执行
  std::vector<t_NpcAIDefine> phaseVector;
  ///当前的AI索引
  DWORD curPhase;
  ///当前AI的结束时间
  zRTime phaseEndTime;

  ///脚本重复次数
  ///-1：无限循环  0：停止  >0：循环次数
  int repeat;

  ///是否起用了脚本
  bool active;
  void nextPhase(int index);
  void on_phaseEnd();
  SceneNpcAIType parseAction(char *);
  
  ///本控制器控制的npc
  SceneNpc * npc;

  ///当前的AI和保存的前一个AI
  t_NpcAIDefine curAI,oldAI;
  ///活动范围的中心
  zPos actPos;
  ///活动范围的宽和高
  int actRegionX,actRegionY;
  bool outOfRegion() const;
  void returnToRegion();

  bool arrived(zPos pos = zPos(0,0),int x = -1,int y = -1);
  bool dstReached();
  ///是否到达目的地
  bool reached;

  ///目的地图
  char dstMap[32];
  ///目的位置
  zPos dstPos;

public:
  void setAI(const t_NpcAIDefine ai,const bool setTime = true);
  void switchAI(const bool setTime = true);
  void setNormalAI();
  void setActRegion(zPos pos = zPos(0,0),int x = -1,int y = -1);
  void getActRegion(zPos &,int &,int  &);
  NpcAIController(SceneNpc *);
  bool loadScript(DWORD id);
  void unloadScript();
  void processPhase();
  void setRepeat(int repeat);
  int getRepeat();
  bool isActive();
  void setPhaseTime(const int delay);
  void delayPhaseTime(const int delay);
  bool phaseTimeOver();

  void on_relive();
  void on_find_enemy(const SceneEntryPk *);
  void on_hit(SceneEntryPk *pAtk);
  void on_die();
};

//需要进行AI动作的npc
typedef std::set<SceneNpc *,std::less<SceneNpc *> > MonkeyNpcs;

/*
struct t_PetState
{
  DWORD id;
  Cmd::petType type;
  char name[MAX_NAMESIZE];
  DWORD hp;
  DWORD exp;
  WORD ai;
};
*/

enum
{
	TYPE_USER = 0,
	TYPE_TEAM = 1,
};

struct t_expRec
{
	WORD IdType;
	DWORD wdHP;
	zTime  attack_time;

	t_expRec()
	{
		IdType = TYPE_USER;
		attack_time.now();
		wdHP = 0;
	}
};

/* 一些特殊npc的ID */
const DWORD COUNTRY_MAIN_FLAG = 58001;
const DWORD COUNTRY_SEC_FLAG  =  58002;
const DWORD COUNTRY_MAIN_GEN = 58200;
const DWORD COUNTRY_SEC_GEN  = 58201;
const DWORD COUNTRY_KING_MAIN_FLAG = 58005;
const DWORD COUNTRY_KING_SEC_FLAG = 58006;
const DWORD COUNTRY_EMPEROR_MAIN_GEN = 58203;
const DWORD COUNTRY_EMPEROR_SEC_GEN = 58204;

const DWORD ALLY_GUARDNPC = 54100;//盟国镖车
const DWORD EMPEROR_HORSE_ID = 3202;//皇帝的马ID
const DWORD KING_HORSE_ID = 3204;//国王的马ID

//sky AI配置事件结构
struct NpcAiCs {
	DWORD id;
	WORD level;
	int target;
	int Rate;
	DWORD cond1;
	DWORD cond2;

	NpcAiCs() {
		id			= 0;
		level		= 0;
		target		= 0;
		Rate		= 0;
		cond1		= 0;
		cond2		= -1;
	}
};

struct SummonNpcData
{
	DWORD id[3];
	WORD num[3];
	SummonNpcData()
	{
		bzero(id, sizeof(DWORD)*3);
		bzero(num, sizeof(WORD)*3);
	}
};

//sky 建筑类NPC特殊事件结构
struct stArchitectureMatter
{
	DWORD SummonTime;
	DWORD levelupTime;
	DWORD SurviveTime;
	WORD AiId;
	std::vector<SummonNpcData> SummonNpc;

	stArchitectureMatter()
	{
		SummonTime = 0;
		levelupTime = 0;
		SurviveTime = 0;
		SummonNpc.clear();
	}
};

//sky AI脚本存储结构
typedef struct stNewAIData
{
	char AiName[33];
	DWORD AiId;
	DWORD aif;
	int npc_search_region;
	std::vector<NpcAiCs> NpcAiList;	
	std::vector<NpcAiCs> NpcAiTimeList;
	stArchitectureMatter Matter;
	stNewAIData()
	{
		AiName[0] = 0;
		AiId = 0;
		NpcAiList.clear();
		NpcAiTimeList.clear();
	}

}stNewAIData_t;

//sky AI脚本存储器
extern std::list<stNewAIData_t> NpcAiXmlData;
extern DWORD MapdwNpcNum;

/**
 * \brief 定义Npc
 *
 */
class SceneNpc : public SceneEntryPk,public zAStar<>,public zAStar<2>
{
  friend class NpcAIController;

  static const DWORD maxUniqueID = 100000;

  public:

  bool isMainGeneral();//是否大将军王

  zRTime reliveTime;//复活时间

  int targetDistance;//与当前目标的最短距离
  int closeCount;//追逐计数,10步以内最短距离没有减少则认为目标不可到达

  std::list<ScenePet *> semipetList;//半宠物列表

  /**
   * \brief 定义Npc跟踪状态
   *
   */
  enum SceneNpcChase
  {
    CHASE_NONE,     /// 没有跟踪状态
    CHASE_ATTACK,   /// 跟踪攻击状态
    CHASE_NOATTACK    /// 普通跟踪状态
  };

  /**
   * \brief Npc类型
   * 静态的还是动态分配的
   */
  enum SceneNpcType
  {
    STATIC,       /// 静态的
    GANG        /// 动态的
  };

  /**
   * \brief Npc基本数据
   *
   */
  zNpcB *npc;

  /**
   * \brief 增强Npc基本数据
   *
   */
  zNpcB *anpc;

  /**
   * \brief Npc定义数据
   *
   */
  const t_NpcDefine *define;

  /**
   * \brief npc当前生命值
   *
   */
  DWORD hp;

  ///上次发送时的hp
  DWORD lasthp;

  ///回血标记
  bool needRecover;
  ///下次回血的时间
  zRTime rcvTimePet;//宠物休息
  zRTime rcvTimeUnder30;//hp30以下
  zRTime rcvTimeRest;//脱离战斗
  //hp30以下回血
  bool recoverUnder30;
  //结束战斗回血
  //bool recoverLeaveBattle;

  bool checkRecoverTime(const zRTime& ct);
  void setRecoverTime(const zRTime& ct,int delay);

  virtual bool recover();
  /// npc的生命结束时间(如果没设置就是其创建时间)
  DWORD dwStandTime;

  ///npc的生命持续时间
  DWORD dwStandTimeCount;

  ///附加最小攻击力
  WORD appendMinDamage;

  ///附加最大攻击力
  WORD appendMaxDamage;

  ///是否已经锁定目标
  bool lockTarget;

  ///攻城的npc,随攻城一起删除
  bool isRushNpc;

  ///根据配置召唤其他npc
  int summonByNpcMap(std::map<DWORD,std::pair<DWORD,DWORD> > map);

  ///是否召唤过
  bool summoned;

  //sky 用于队长分配时需要用到
  std::list<zObject *> m_TemObj;	//分配物品列表
  DWORD Captain_ID;					//队长ID


  bool setCurTarget(SceneEntryPk *,bool=false);
  bool setCurTarget(DWORD,DWORD,bool=false);
  void leaveBattle(bool bDie=false);

  void setClearState();
  bool needClear();

  bool isSpecialNpc();
  bool isFunctionNpc();
  bool isTaskNpc();

  SceneNpc(Scene *scene,zNpcB *npc,const t_NpcDefine *define,const SceneNpcType type,const SceneEntryType entrytype=SceneEntry_NPC,zNpcB *anpc=NULL);
  ~SceneNpc();
  bool zPosOutofRange()const;
  SceneNpcChase getChaseMode()const;
  SceneEntryPk* getChaseSceneEntry() const;
  bool chaseSceneEntry(const DWORD type,const DWORD userid);
  virtual bool isRedNamed(bool allRedMode=true);
  void unChaseUser();
  bool checkLockUserOverdue(const zRTime &ct);
  void setLockUser(const DWORD dwID);
  bool checkMoveTime(const zRTime &ct);
  void setMoveTime(const zRTime &ct);
  void setMoveTime(const zRTime &ct,const int delay);
  void delayMoveTime(const int delay);
  bool checkAttackTime(const zRTime &ct) const;
  void setAttackTime(const zRTime &ct);
  void setAttackTime(const zRTime &ct,const int delay);
  //bool checkUnchaseTime(const zRTime &ct) const;
  //void setUnchaseTime(const zRTime &ct,const int delay);
  bool canLostObject(const zRTime &ct);
  bool canRelive(const zRTime &ct);
  bool canRush();
  const SceneNpcType &getSceneNpcType() const;
  void full_t_NpcData(Cmd::t_NpcData &data);
  void full_t_MapNpcDataState(Cmd::t_MapNpcDataState &data);
  void full_t_MapNpcDataAndPosState(Cmd::t_MapNpcDataPosState &data);
  void full_t_MapNpcData(Cmd::t_MapNpcData &data);
  void full_stRTMagicPosUserCmd(Cmd::stRTMagicPosUserCmd &ret) const;
  void full_t_MapNpcDataAndPos(Cmd::t_MapNpcDataPos &data);
  virtual void sendMeToNine();
  bool isBugbear();
  bool canBeAttack();
  void death(const zRTime &ct);
  void backoff(const int direct,const int step);
  void reduceHP(SceneUser *pAtt,DWORD wdHP);
  void distributeExp();
  void distributeMoney(DWORD money);
  void clearStateToNine(WORD state);
  void setStateToNine(WORD state);
  /**
   * \brief 让宠物重生
   */
  virtual void relivePet(){};

  DWORD catchme; ///吸引怪物攻击自己
  int boostupPet; /// 增强比例
  DWORD boostupPetMDef; //增强宠物的法术防御
  DWORD boostupSummon; ///召唤兽攻击加强
  DWORD boostupHpMaxP;  ///增加生命值上限
  DWORD dwReduceDam;  /// 召唤兽伤害扣减
  DWORD giddy;   ///攻击的时候使对方眩晕的几率

  BYTE notifystep; //绿BOSS通知步骤

  //*
  static void AI(const zRTime& ctv,MonkeyNpcs &affectNpc,const DWORD group,const bool every);

  /**
   * \brief 强制跟踪用户,如果怪已经在跟踪用户,那么有45%的几率将目标转换成目前的用户
   * \param pAtt 要跟踪的对象
   */
  bool forceChaseUser(SceneEntryPk *pAtt);
  /**
   * \brief 将客户端消息转发到会话服务器
   */
  bool forwardSession(const Cmd::stNullUserCmd *pNullCmd,const DWORD nCmdLen);
  //bool checkChaseUser(SceneUser *&sceneUser);
  bool checkChaseAttackTarget(SceneEntryPk *&entry);
  bool canChaseTarget(const SceneEntryPk *entry);
  bool canAttack(SceneEntryPk *entry);
  bool inRange(SceneEntryPk *entry);
  bool attackTarget(SceneEntryPk *entry = NULL);
  //bool chasedFindFrontUser( const int direct);
  //bool findAndChaseUser(const int radius,SceneEntryPk *&entry);
  //bool findAnyTarget(const int radius,SceneEntryPk *&entry);
  void action(const zRTime& ctv);
  virtual bool normalAction();
  bool deathAction();
  bool hideAction();
  bool moveable(const zPos &tempPos,const zPos &destPos,const int radius);
  bool move(const int direct,const int step);
  bool warp(const zPos &pos,bool ignore=false);//跳转
  void jumpTo(zPos &newPos);
  bool gotoFindPath(const zPos &srcPos,const zPos &destPos);
  bool goTo(const zPos &pos);
  bool shiftMove(const int direct);
  void set_quest_status(SceneUser* user);
  void setStandingTime(DWORD standTime);
  void refreshExpmapAttackTime(SceneUser* pAtt);

  /**
   * \brief 改变角色的hp
   * \param hp 变更的HP
   */
  void changeHP(const SDWORD &hp);

  /**
   * \brief 造成直接伤害
   * \param pAtt 攻击者
   * \param dam 伤害值
   * \param notify 通知伤害显示
   */
  SWORD directDamage(SceneEntryPk *pAtt,const SDWORD &dam,bool notify=false);

  /**
   * \brief 改变角色的sp
   * \param sp 变更的SP
   */
  void changeSP(const SDWORD &sp);

  /**
   * \brief 改变角色的mp
   * \param mp 变更的MP
   */
  void changeMP(const SDWORD &mp);

  /**
   * \brief 在被自己攻击之前的处理,包括,装备损耗处理,攻击有效几率判断等
   * \param pUser 攻击者
   * \param rev 本次攻击的触发指令
   * \param physics 是否物理攻击
   * \param good 为true则必中,为false需要判断回避率
   * \return true为这次攻击是有效的,false为一次无效的攻击
   */
  bool preAttackMe(SceneEntryPk *pUser,const Cmd::stAttackMagicUserCmd *rev,bool physics=true,const bool good = false);

  /**
   * \brief 角色被攻击
   * \param pEntry 攻击者
   * \param rev 本次攻击的触发消息
   * \param physics 是否物理攻击
   * \return true为这次攻击是有效的,false为一次无效的攻击
   */
  bool AttackMe(SceneEntryPk *pEntry,const Cmd::stAttackMagicUserCmd *rev,bool physics=true,SWORD rangDamageBonus=0);

  /**
   * \brief 角色被击退N格
   * \param dwAttTempID 攻击者的临时ID
   * \param grids 后退几格
   */
  void standBack(const DWORD dwAttTempID,DWORD grids);

  /**
   * \brief 将攻击目标换成dwTempID所指向的角色玩家
   * \param dwTempID 目标角色的临时ID
   */
  void changeAttackTarget(const DWORD &dwTempID);

  /**
   * \brief 让角色死亡
   */
  void toDie(const DWORD &dwTempID);

  /**
   * \brief 通知客户端生命值的变化
   */
  void attackRTHpAndMp();

  /**
   * \brief 发送npc血槽
   */
  void showHP(SceneUser *pUser,DWORD npchp);

  /**
   * \brief 检查玩家是否攻击过该npc
   */
  bool isAttackMe(SceneEntryPk *);
  /**
   * \brief 判断角色是否死亡
   * \return true为死亡
   */
  bool isDie();
  /**
   * \brief 获取角色的级别
   */
  virtual DWORD getLevel() const;

  /**
   * \brief 需要的职业类型,决定可以使用的技能类型
   */
  bool needType(const DWORD &needtype);

  /**
   * \brief 需要的职业类型,决定可以使用的技能类型
   */
  bool addSkillToMe(zSkill *skill);

  /**
   * \brief 是否有该技能需要的武器
   * \return true 有 false 没有
   */
  bool needWeapon(DWORD skillid);

  /**
   * \brief 是否Pk区域
   * \other PK相关人
   * \return true 是 false 否
   */
  virtual bool isPkZone(SceneEntryPk *other=NULL);

  /**
   * \brief 依赖物品消耗型法术
   * \param object 消耗物品的类型
   * \param num 消耗物品的数量
   * \return true 消耗成功 false 失败
   */
  bool reduce(const DWORD &object,const BYTE num);

  /**
   * \brief 检查可消耗物品是否足够
   * \param object 消耗物品的类型
   * \param num 消耗物品的数量
   * \return true 足够 false 不够
   */
  bool checkReduce(const DWORD &object,const BYTE num);

  /**
   * \brief 施放技能所导致的消耗MP,HP,SP
   * \param base 技能基本属性对象
   * \return true 消耗成功 false 失败
   */
  bool doSkillCost(const zSkillB *base);

  /**
   * \brief 检查施放技能所导致的消耗MP,HP,SP是否足够
   * \param base 技能基本属性对象
   * \return true 消耗成功 false 失败
   */
  bool checkSkillCost(const zSkillB *base);

  /**
   * \brief 检查自身的施放成功几率,决定这次技能是否可以施放
   * \return true 成功 false 失败
   */
  bool checkPercent();

  /**
   * \brief 判断是否是敌人
   * \return true 是 false 不是
   */
  //bool isEnemy(SceneUser *pUser);

  /**
   * \brief 死亡后给特定角色增加额外经验,包括护保和装备的额外经验
   * \param wdExp 分配到的经验
   * \param pUser 分配对象
   * \return 重新计算后的经验值
   */
  DWORD addOtherExp(DWORD wdExp,SceneUser *pUser);

  /**
   * \brief 重置最大的hp
   */
  virtual void changeAndRefreshHMS(bool lock=true,bool sendData=true);

  /**
   * \brief 根据等级差别重新计算经验
   * \param wdExp 分配到的经验值
   * \param char_level 角色等级
   * \return 重新计算后的经验值
   */
  DWORD levelExp(DWORD wdExp,DWORD char_level);

  /**
   * \brief 重新发送本NPC的地图数据
   */
  void reSendMyMapData();

  //设置宠物的主人
  virtual void setMaster(SceneEntryPk *){}

  //获取宠物的主人
  virtual SceneEntryPk *getMaster() {return 0;}
  /**
   * \brief 得到最上层的主人
   *
   * \return 主人
   */
  virtual SceneEntryPk *getTopMaster(){return this;}

  //切换场景
  bool changeMap(Scene *newScene,const zPos &pos);
  void setAspeedRate(float rate);
  void resetAspeedRate();

  /**
   * \brief 通知选中自己的用户的hp和mp发生变化
   */
  void sendtoSelectedHpAndMp();
  void sendtoSelectedState(DWORD state,WORD value,WORD time);

  bool createRush();
  virtual bool moveToMaster();

  //AI相关的方法
  void setSpeedRate(float rate);
  float getSpeedRate();
  void resetSpeedRate();

  //const t_NpcAIDefine& getAI();
  void setAI(const t_NpcAIDefine &ad);
  //void setActRegion(zPos pos,DWORD regionX,DWORD regionY);
  //void setNormalAI();
  bool doNormalAI();
  bool doSayAI();
  bool doRandomChatAI();
  bool doMovetoAI();
  bool doAttackAI();
  bool doGoOnPathAI();
  bool doPatrolAI();
  bool doFleeAI();
  bool doChangeMapAI();
  bool doWarpAI();
  bool doReturnToRegionAI();
  bool doClearAI();
  bool doDropItemAI();
  //void returnToRegion();

  //[sky]扩充NPC AI函数
  bool m_bFighting;		//NPC是否处于战斗中的标志
  DWORD m_dwNowTime;
  DWORD m_dwGtime;		//记录战斗持续的时间(秒级)
  bool m_mobType;		//召唤怪物标志

  struct m_NpcSkill{    
	  int SkillType;
	  SceneEntryPk * target; 
	  DWORD id; 
	  WORD level;

	  m_NpcSkill() {
		  SkillType		= 0;
		  target		= NULL; 
		  id			= 0; 
		  level			= 0;
	  }
  };

  std::vector<m_NpcSkill> m_SkillList;  //NPC需要施放的技能列表

  //////////////////////////////////////////////////////////////////////////////////
  //sky NPC变身处理接口
  bool ChangeNpc();

  //sky 建造时间(建筑类型NPC用)
  DWORD MakeTime;
  //////////////////////////////////////////////////////////////////////////////////
  std::vector<NpcAiCs> NpcAiList;		//sky 条件AI列表
  std::vector<NpcAiCs> NpcAiTimeList;	//sky 定时AI列表

  virtual bool GetNpcAi(); //sky 获取AI
  virtual bool laod_ai();  //sky 读取AI脚本
  bool del_ai();	//sky 清除现在的AI(用来NPC变身后重新载入AI前初始化用)
  //////////////////////////////////////////////////////////////////////////////////
  //sky 检测是否是建筑子类NPC类对象
  virtual bool IsArchitecture() {return false;}
  //sky 建筑类time函数调用
  virtual void ArchitecTimer(const zRTime& ctv) {};

  //sky 检测是否超过设定的随机范围
  virtual bool outOfRandRegion(zPos * pos);

  bool lua_useSkill( DWORD id, WORD level, int target, int Rate, DWORD cond1, DWORD cond2 = -1 );
  bool lua_timeSkill( DWORD id, WORD level, int target, int Rate, DWORD time1, DWORD time2 );
  bool Detection_cond( SceneEntryPk * target, DWORD cond1, DWORD cond2 );

  virtual bool isRedNamed(bool allRedMode=true) const;
  bool useSkill( SceneEntryPk * target,DWORD id, WORD level = 0 );		//[sky]添加技能等级参数
  bool useMobSkill(SceneEntryPk * target, DWORD id, DWORD mobID);  //[sky]添加NPC召唤属下函数
  virtual SceneEntryPk * chooseEnemy(SceneEntryPk_vec &);
  bool debuffEnemy(SceneEntryPk *);
  bool attackEnemy(SceneEntryPk *);
  bool moveToEnemy(SceneEntryPk *);
  virtual bool runOffEnemy(SceneEntryPk_vec &);
  virtual bool isActive();
  virtual bool canMove();
  virtual bool canFight();
  //virtual bool checkMasterTarget(SceneEntryPk *&entry){entry = 0;return false;}
  virtual void petDeath(){}
  bool healSelf();
  bool buffSelf();

  bool canReach(SceneEntryPk *);
  bool isSurrounded();
  int isEnemy(SceneEntryPk *,bool notify = false,bool good = false);
  bool getEntries(int,SceneEntryPk_vec &,int);
  bool healFellow(SceneEntryPk_vec &);
  bool buffFellow(SceneEntryPk_vec &);
  bool helpFellow(SceneEntryPk_vec &);

  virtual bool randomMove();
  void randomChat(NpcChatType type);

  //回调点
  virtual void on_reached() { }
  virtual void on_death(SceneEntryPk* att){}
  virtual void check() { }
  //sky Add begin
  bool dreadProcess();				//sky 控制恐惧的胡乱移动
  bool blindProcess();				//sky 控制失明的胡乱移动
  void reflectSkill(SceneEntryPk *pAtt,const Cmd::stAttackMagicUserCmd *rev);	//sky 反射技能
  void reflectDam(int &dwDamDef,int &dwDamSelf,DWORD skillID);					//sky 反弹伤害
  //sky end
  int IsOppose(DWORD five);

  virtual void clearMaster();

  //zPos actPos;//活动位置
  //DWORD actRegionX,actRegionY;//活动范围
  //t_NpcAIDefine oldAI;
  ///npc控制器的指针
  NpcAIController * AIC;
  bool setScript(int id);
  void clearScript();
  void assaultMe(BYTE attacktype,DWORD tempid);

  BYTE getAType();

  ///npcAI标志
  DWORD aif;
  ///宠物的AI模式
  //WORD petAI;
  ScenePet *	summonPet(DWORD,Cmd::petType,DWORD,DWORD,const char *,DWORD,zPos pos=zPos(0,0),BYTE dir=4);
  //sky 元神召唤虚函数
  bool			MirageSummonPet(DWORD id,Cmd::petType type,DWORD standTime,WORD num,const char * petName = "",DWORD anpcid = 0,zPos pos=zPos(0,0),BYTE dir=4) { return false; }
  //sky 召唤士兵虚函数
  virtual int summonSoldiers(DWORD id, Cmd::petType type, WORD num, DWORD sid=0, const char * name="", DWORD anpcid = 0,zPos pos=zPos(0,0),BYTE dir=4) {return NULL;}

  bool killOnePet(ScenePet *);
  void killAllPets();

  virtual void addPetExp(DWORD num,bool addPet,bool addCartoon){}
  
  virtual void full_PetDataStruct(Cmd::t_PetData &);
  virtual void setPetAI(Cmd::petAIMode){}
  virtual Cmd::petType getPetType();
  virtual void setPetType(Cmd::petType){}
  virtual void setAppendDamage(WORD mindamage,WORD maxdamage);
  virtual DWORD getMinMDamage();
  virtual  DWORD getMaxMDamage(); 
  virtual DWORD getMinPDamage();
  virtual  DWORD getMaxPDamage();
  virtual DWORD getMasterMana(){return 0;}
  virtual DWORD getMinMDefence();
  virtual DWORD getMaxMDefence(); 
  virtual DWORD getMinPDefence();
  virtual DWORD getMaxPDefence();
  virtual DWORD getMaxHP();
  virtual DWORD getBaseMaxHP();

  ///次攻击目标
  DWORD secondTargetType;
  DWORD secondTargetID;
  bool setSecondTarget(SceneEntryPk *);
  bool setSecondTarget(DWORD,DWORD);
  bool chaseSecondTarget();
  bool chaseItsMaster();
  SceneEntryPk * getSecondTarget();
  void hideMe(int showDelay);
  void goToRandomScreen();

  /**
   * \brief 物品保护
   *
   */
  DWORD dwNpcLockedUser;

  /// NPC搜索敌人的范围
  int npc_search_region; //sky 现在可以被外部配置文件改变所以移到公有成员

protected:
  
  /// NPC远离目标放弃追踪的范围
  static const int npc_lost_target_region = 12;
  /// NPC远离活动范围放弃追踪的距离
  static const int npc_out_of_range_region = 20;
  /// 宠物保持在主人身边的范围
  static const int npc_pet_chase_region = 2;
  /// 宠物离主人超过此距离则加速
  static const int npc_pet_run_region = 4;
  /// 宠物离主人超过此距离则跳转
  static const int npc_pet_warp_region = 6;
private:
  /**
   * \brief NPC可清除
   *
   */
  bool clearMe;

  ///npc当前的AI
  t_NpcAIDefine AIDefine;

  ///npc的主人
  //SceneUser * master;

  ///移动速度倍率
  float speedRate;
  ///攻击速度倍率
  float aspeedRate;

  ///是否已经因为hp<20%提升了移动速度
  bool speedUpUnder20;

  ///是否已经因为hp<50%提升了攻击速度
  bool aspeedUpUnder50;


  bool processDeath(SceneEntryPk *pAtt);

  ///是否正在后退中(ms)
  int backOffing;

  /**
   * \brief 第一次被攻击时间(测试用
   *
   */
  /// 第一次被攻击的时间
  zRTime first_time;

  ///半秒定时器
  Timer _half_sec;

  ///1秒定时器
  Timer _one_sec;

  ///3秒定时器
  Timer _3_sec;

  /**
   * \brief 跟踪方式
   *
   */
  ///npc的跟踪方式
  SceneNpcChase chaseMode;

  //DWORD  dwNpcChasedEntryType;
  /**
   * \brief 所跟踪目标的的编号
   *
   */
  //DWORD curTarget;
  //DWORD  dwNpcChasedEntryID;

  /**
   * \brief 物品保护时间
   *
   */
  zRTime lockedUserTime;
  /**
   * \brief 下一次移动时间
   *
   */
  zRTime nextMoveTime;
  /**
   * \brief 下一次攻击时间
   *
   */
  zRTime nextAttackTime;

  ///结束隐身状态的时间
  zRTime showTime;

  /**
   * \brief 是否可以掉落物品
   *
   */
  bool lostObject;

  //是否进行怪物攻城的判断
  bool mayRush;

  /**
   * \brief Npc类型
   * 静态的还是动态分配的
   */
  const SceneNpcType type;

  /**
   * \brief 临时编号的线性分配器
   * 主要在创建静态Npc的时候需要使用
   */
  static DWORD serialID;
  /**
   * \brief 临时编号的唯一分配器
   * 主要在创建动态Npc的时候需要使用
   */
  static zUniqueDWORDID uniqueID;

  //typedef hash_map<DWORD,t_expRec> NpcHpHashmap;
  typedef std::map<DWORD,t_expRec> NpcHpHashmap;
  typedef NpcHpHashmap::iterator NpcHpHashmap_iterator;
  typedef NpcHpHashmap::const_iterator NpcHpHashmap_const_iterator;
  typedef NpcHpHashmap::value_type NpcHpHashmap_pair;
  ///经验值列表
  ///可以分到该npc经验的玩家列表
  NpcHpHashmap expmap;


public:
  /**
   * \brief 设置角色的当前状态,并根据当前状态呈现角色的特效或者关闭特效
   * \param state 状态ID 根据enum SceneEntryState取值
   * \param isShow 是否显示效果
   * \return true为这次攻击是有效的,false为一次无效的攻击
   */
  void showCurrentEffect(const WORD &state,bool isShow,bool notify=true);
  /**
   * \brief NPC尸体的使用状态
   * true为已经被使用
   */
  bool isUse;
};

/**
 * \brief 对每个特殊npc执行的回调
 *
 */
struct specialNpcCallBack
{
  public:
    virtual bool exec(SceneNpc *npc)=0;
    virtual ~specialNpcCallBack(){};
};

/**
 * \brief 召唤一个npc
 * \param define npc定义结构
 * \param pos 召唤位置
 * \param base npc基本信息
 * \param standTime 图腾系的持续时间
 * \param abase 增强npc的基本信息
 * \return 召唤出npc的指针,失败返回0
 */
  template <typename Npc>
Npc* Scene::summonOneNpc(const t_NpcDefine &define,const zPos &pos,zNpcB *base,unsigned short dupIndex,DWORD standTime,zNpcB* abase,BYTE vdir, SceneEntryPk * m)
{
  t_NpcDefine *pDefine = new t_NpcDefine(define);
  if (pDefine)
  {
	  int entrytype;
	  entrytype = zSceneEntry::SceneEntry_NPC;

	  Npc *sceneNpc = new Npc(this,base,pDefine,SceneNpc::GANG,(const zSceneEntry::SceneEntryType)entrytype ,abase);
	  sceneNpc->dupIndex = dupIndex;
	  if (sceneNpc)
	  {
		  //sky 读取AI文件
		  sceneNpc->GetNpcAi();
		  sceneNpc->setDir(vdir);
		  sceneNpc->setStandingTime(standTime);
		  initNpc(sceneNpc,NULL,pos);//zPos(0,0));//填NULL则在define.region范围内选择位置
		  if (sceneNpc->getState() == zSceneEntry::SceneEntry_Normal)
		  {
			  if (base->kind != NPC_TYPE_TRAP)
			  {
				  if( base->kind == NPC_TYPE_GHOST )
				  {
					  if( m )
					  {
						  SceneUser *entry = (SceneUser *)m;
						  Cmd::stAddUserAndPosMapScreenStateUserCmd addNpc;
						  ((SceneGhost *)sceneNpc)->full_t_MapUserDataPosState( addNpc.data, entry ); //设置地图添加元神的消息
						  //sendCmdToNine(sceneNpc->getPosI(),&addNpc,sizeof(addNpc),sceneNpc->dupIndex);
						  sendCmdToNineExceptMe( entry, sceneNpc->getPosI(),&addNpc,sizeof(addNpc) );

						  //sky为主人发送有自定颜色的元神(以给本身做区别)
						  addNpc.data.dwBodyColorSystem = 0x600000ff; 
						  entry->sendCmdToMe(&addNpc,sizeof(addNpc));

						  Cmd::stRTMagicPosUserCmd ret;
						  sceneNpc->full_stRTMagicPosUserCmd(ret);
						  sendCmdToNine(sceneNpc->getPosI(),&ret,sizeof(ret),sceneNpc->dupIndex);
					  }
				  }
				  else
				  {
					  Cmd::stAddMapNpcMapScreenUserCmd addNpc;
					  sceneNpc->full_t_MapNpcData(addNpc.data);
					  sendCmdToNine(sceneNpc->getPosI(),&addNpc,sizeof(addNpc),sceneNpc->dupIndex);
					  Cmd::stRTMagicPosUserCmd ret;
					  sceneNpc->full_stRTMagicPosUserCmd(ret);
					  sendCmdToNine(sceneNpc->getPosI(),&ret,sizeof(ret),sceneNpc->dupIndex);
				  }
			  }
			  else
			  {
				  SceneEntryPk *entry = sceneNpc->getMaster();
				  if (entry&& entry->getType() == zSceneEntry::SceneEntry_Player)
				  {
					  SceneUser *pUser = (SceneUser *)entry;
					  Cmd::stAddMapNpcMapScreenUserCmd addNpc;
					  sceneNpc->full_t_MapNpcData(addNpc.data);
					  pUser->sendCmdToMe(&addNpc,sizeof(addNpc));
					  Cmd::stRTMagicPosUserCmd ret;
					  sceneNpc->full_stRTMagicPosUserCmd(ret);
					  pUser->sendCmdToMe(&ret,sizeof(ret));
				  }
			  }
		  }
#ifdef _DEBUG
		  else
			  Zebra::logger->debug("%s 初始状态 %u",sceneNpc->name,sceneNpc->getState());
#endif
		  return sceneNpc;
	  }
	  else
	  {
		  Zebra::logger->fatal("Scene::summonOneNpc:SceneNpc分配内存失败");
		  SAFE_DELETE(pDefine);
	  }
  }
  else
  {
	  Zebra::logger->fatal("Scene::summonOneNpc:t_NpcDefine分配内存失败");
  }
  return NULL;
}

struct petBonus
{
  DWORD type;
  WORD atkB;
  WORD defB;
  WORD hpB;

  /*
  petBonus()
  {
    type = 0;
    atkB = 0;
    defB = 0;
    hpB = 0;
  }
  petBonus(DWORD p,WORD a,WORD d,WORD h)
  {
    type = p;
    atkB = a;
    defB = d;
    hpB = h;
  }
  petBonus(petBonus & pb)
  {
    type = pb.type;
    atkB = pb.atkB;
    defB = pb.defB;
    hpB = pb.hpB;
  }
  */
};

class ScenePet : public SceneNpc
{
private:
	/*
	static std::map<DWORD,petBonus> bonusTable;
	*/

	///主人
	//SceneEntryPk * master;
	DWORD masterID;
	DWORD masterType;

	DWORD delCount;

	///宠物类型
	Cmd::petType type;
	///宠物的行动模式
	//WORD petAI;

	///等级
	//DWORD level;

	///是否因为离主人太远而提升了速度
	bool speedUpOffMaster;
	int isUserMasterEnemy(SceneEntryPk *);
public:

	//sky 宠物最大等级
	WORD maxlevel;

	//bool needSave;
	ScenePet(Scene* scene,zNpcB *npc,const t_NpcDefine *define,const SceneNpcType type,const SceneEntryType entrytype=SceneEntry_NPC,zNpcB *abase=NULL);
	//[sky] 召唤士兵(每个士兵都是一个独立的个体不同以召唤兽或者宠物)
	virtual	int summonSoldiers(DWORD id, Cmd::petType type, WORD num, DWORD sid=0, const char * name="", DWORD anpcid = 0,zPos pos=zPos(0,0),BYTE dir=4) {return NULL;}
	Cmd::t_PetData petData;

	void setPetAI(Cmd::petAIMode);
	WORD getPetAI();
	int isEnemy(SceneEntryPk *,bool = false,bool good = false);
	SceneEntryPk * chooseEnemy(SceneEntryPk_vec &);
	bool isRedNamed(bool allRedMode=true);
	void returnToRegion();
	void full_PetDataStruct(Cmd::t_PetData &);
	virtual bool moveToMaster();
	bool randomMove();
	bool isActive();
	bool masterIsAlive;
	virtual bool canMove();
	bool canFight();
	bool runOffEnemy(SceneEntryPk_vec&);

	bool checkMasterTarget(SceneEntryPk *&entry);
	void setMaster(SceneEntryPk *);
	void setMaster(DWORD,DWORD);
	SceneEntryPk * getMaster();
	DWORD getMasterID() const{return masterID;}
	SceneEntryPk * getTopMaster();
	void clearMaster();

	Cmd::petType getPetType();
	void setPetType(Cmd::petType);
	void petDeath();
	virtual void sendData();
	virtual void sendHpExp();
	virtual DWORD getLevel() const;
	virtual bool addExp(DWORD);
	void addPetExp(DWORD);

	bool isPkZone(SceneEntryPk *other=NULL);
	bool recover();

	//给GuardNpc继承用
	virtual void reset(){}
	virtual void check(){};
	virtual void on_death(SceneEntryPk* att){}
	virtual void setAppendDamage(WORD mindamage,WORD maxdamage);
	DWORD getMasterMana();
	void changeAndRefreshHMS(bool lock=true,bool sendData=true);

	virtual DWORD getMaxHP();
	virtual DWORD getBaseMaxHP();
	virtual DWORD getMinMDamage();
	virtual DWORD getMaxMDamage(); 
	virtual DWORD getMinPDamage();
	virtual DWORD getMaxPDamage();
	virtual DWORD getMinMDefence();
	virtual DWORD getMaxMDefence();
	virtual DWORD getMinPDefence();
	virtual DWORD getMaxPDefence();

	virtual void levelUp();
	void getAbilityByLevel(DWORD);

	virtual bool normalAction(){return SceneNpc::normalAction();}

	void full_t_MapPetData(Cmd::t_MapPetData &data);

	virtual void sendMeToNine();
	virtual void sendPetDataToNine();

	virtual void delMyself();

	virtual bool IsArchitecture() {return false;}
};


/**
 * \brief 元神类型NPC类的实现 [sky]
 */
class SceneGhost: public ScenePet
{
public:
	SceneGhost(Scene* scene,zNpcB *npc,const t_NpcDefine *define,const SceneNpcType type,const SceneEntryType entrytype=SceneEntry_NPC,zNpcB *abase=NULL);
	~SceneGhost( ) {}
	void full_t_UserData(Cmd::t_UserData &data, SceneUser * master);						//填充本身的数据数据 [sky]
	void full_t_MapUserDataPosState( Cmd::t_MapUserDataPosState &data, SceneUser * master );	//填充t_MapUserDataPosState
	void full_t_MapUserData(Cmd::t_MapUserData &data, SceneUser * master);
	void full_zNpcB( SceneUser * master );		//获取主人的数据来填充元神数据
	virtual bool IsArchitecture() {return false;}
};

/**
 * \brief npc管理器
 *
 */
class SceneNpcManager : public zEntryManager< zEntryTempID >
{
  public: 

    bool init();
    bool addSceneNpc(SceneNpc *sceneNpc);
    bool addSpecialNpc(SceneNpc *,bool=false);
    void removeSceneNpc(SceneNpc *sceneNpc);

    SceneNpc *getNpcByTempID(DWORD tempid);
    static SceneNpcManager &getMe();
    static void destroyMe();
    /**
     * \brief 对每个npc执行回调函数
     *
     *
     * \param exec 回调函数
     * \return 是否继续执行
     */
    template <class YourNpcEntry>
    bool execEveryNpc(execEntry<YourNpcEntry> &exec)
    {
      rwlock.rdlock();
      bool ret=execEveryEntry<>(exec);
      rwlock.unlock();
      return ret;
    }

    /**
     * \brief 删除符合条件的npc
     *
     * \param pred 判断条件
     */
    template <class YourNpcEntry>
    void removeNpc_if(removeEntry_Pred<YourNpcEntry> &pred)
    {
      rwlock.wrlock();
      removeEntry_if<>(pred);
      rwlock.unlock();
    }

    /**
     * \brief 删除一场景内的npc
     *
     *
     * \param scene 场景
     */
    void removeNpcInOneScene(Scene *scene);
    void SpecialAI();
    /**
     * \brief 对每个特殊npc执行回吊调函数
     *
     *
     * \param callback 回调函数
     * \return 
     */
    void execAllSpecialNpc(specialNpcCallBack &callback)
    {
      rwlock.wrlock();
      for (MonkeyNpcs::iterator it=specialNpc.begin(); it!=specialNpc.end(); it++)
      {
        if (!callback.exec(*it))
        {
          rwlock.unlock();
          return;
        }
      }
      rwlock.unlock();
    }

    MonkeyNpcs &getSepcialNpc()
    {
      return specialNpc;
    }
    bool getNpcCommonChat(DWORD type,char * content);
    void removeSpecialNpc(SceneNpc *);
    void closeFunctionNpc();
  private:
    ///specialNpc是指即使没有玩家在附近也要做处理的NPC
    ///包括宠物、boss、任务NPC
    MonkeyNpcs specialNpc;

    SceneNpcManager();
    ~SceneNpcManager();

    ///SceneNpcManager的唯一实例
    static SceneNpcManager *snm;
    ///读写锁
    zRWLock rwlock;

    bool getUniqeID(DWORD &tempid) { return true; }       
    void putUniqeID(const DWORD &tempid) {}

    ///npc随机说话的内容
    std::map<DWORD,std::vector<std::string> > NpcCommonChatTable;
    ///npc随机说话的概率
    std::map<DWORD,int> NpcCommonChatRate;

    bool loadNpcCommonChatTable();
};

/**
 * \brief 服务器连接任务
 *
 */
class SceneTask : public zTCPTask,public zEntry,public MessageQueue
{

  public:

    /**
     * \brief 构造函数
     *
     * \param pool 所属连接池指针
     * \param sock TCP/IP套接口
     * \param addr 地址
     */
    SceneTask(
        zTCPTaskPool *pool,
        const SOCKET sock,
        const struct sockaddr_in *addr = NULL) : zTCPTask(pool,sock,addr)
    {
      wdServerID    = 0;
      wdServerType  = UNKNOWNSERVER;
      recycle_state = 0;
      veriry_ok     = false; 
	  // [ranqd] 监视GateWay连接底层收发情况
	  //zSocketMonitor::getInstance().Follow( mSocket );
    }

    /**
     * \brief 虚析构函数
     *
     */
    virtual ~SceneTask();

    int verifyConn();
    int waitSync();
    int recycleConn();
    bool uniqueAdd();
    bool uniqueRemove();
    bool msgParse(const Cmd::t_NullCmd *,const DWORD);
    bool cmdMsgParse(const Cmd::t_NullCmd *,const DWORD);

    /**
     * \brief 返回服务器编号
     *
     * 编号在一个区中是唯一的，保存在管理服务器中
     *
     * \return 服务器编号
     */
    const WORD getID() const
    {
      return wdServerID;
    }

    /**
     * \brief 返回服务器类型
     *
     * \return 服务器类型
     */
    const WORD getType() const
    {
      return wdServerType;
    }
    bool checkRecycle();

  private:

    bool usermsgParse(SceneUser *pUser,const Cmd::t_NullCmd *pNullCmd,const DWORD cmdLen);
    bool usermsgParseBill(SceneUser *pUser,const Cmd::t_NullCmd *pNullCmd,const DWORD cmdLen);
    bool loginmsgParse(const Cmd::t_NullCmd *pNullCmd,DWORD cmdLen);
    WORD wdServerID;
    WORD wdServerType;

    bool verifyLogin(const Cmd::Scene::t_LoginScene *ptCmd);
    int recycle_state;
    bool veriry_ok;


};

/**
 * \brief 场景服务器子连接管理器
 *
 */
class SceneTaskManager : private zNoncopyable
{

  public:

    /**
     * \brief 析构函数
     *
     */
    ~SceneTaskManager() {};

    /**
     * \brief 获取场景子连接管理器唯一实例
     *
     * \return 场景子连接管理器唯一实例
     */
    static SceneTaskManager &getInstance()
    {
      if (NULL == instance)
        instance = new SceneTaskManager();

      return *instance;
    }

    /**
     * \brief 释放类的唯一实例
     *
     */
    static void delInstance()
    {
      SAFE_DELETE(instance);
    }

    bool uniqueAdd(SceneTask *task);
    bool uniqueRemove(SceneTask *task);
    SceneTask *uniqueGet(WORD wdServerID);
    void execEvery();
    bool broadcastCmd(const void *pstrCmd,const int nCmdLen);
    template <class YourEntry>
    void execEverySceneTask(execEntry<YourEntry> &exec)
    {
      rwlock.rdlock();
      SceneTaskHashmap_iterator it;
      it = sceneTaskSet.begin();
      for (; it != sceneTaskSet.end() ; it ++)
      {
        exec.exec(it->second);
      }
      rwlock.unlock();
    }

  private:

    /**
     * \brief 类的唯一实例指针
     *
     */
    static SceneTaskManager *instance;

    /**
     * \brief 构造函数
     *
     */
    SceneTaskManager() {};

    /**
     * \brief 子连接管理容器类型
     *
     */
    typedef hash_map<WORD,SceneTask *> SceneTaskHashmap;
    /**
     * \brief 定义容器迭代器类型
     *
     */
    typedef SceneTaskHashmap::iterator SceneTaskHashmap_iterator;
    /**
     * \brief 定义容器常量迭代器类型
     *
     */
    typedef SceneTaskHashmap::const_iterator SceneTaskHashmap_const_iterator;
    /**
     * \brief 定义容器键值对类型
     *
     */
    typedef SceneTaskHashmap::value_type SceneTaskHashmap_pair;
    /**
     * \brief 容器访问互斥变量
     *
     */
    zRWLock rwlock;
    /**
     * \brief 子连接管理容器类型
     *
     */
    SceneTaskHashmap sceneTaskSet;

};

/**
 * brief 定义 档案服务器连接客户端类
 *
 * 负责 与档案服务器交互，存取档案
 * TODO 暂时只有一个档案服务器
 * 
 */
class RecordClient : public zTCPBufferClient,public MessageQueue
{

  public:

    /**
     * \brief 构造函数
     * 由于档案数据已经是压缩过的，故在底层传输的时候就不需要压缩了
     * \param name 名称
     * \param ip 地址
     * \param port 端口
     */
    RecordClient(const std::string &name,const std::string &ip,const WORD port)
      : zTCPBufferClient(name,ip,port,false) {};

    bool connectToRecordServer();

    void run();
    bool msgParse(const Cmd::t_NullCmd *pNullCmd,const DWORD nCmdLen);
    bool cmdMsgParse(const Cmd::t_NullCmd *,const DWORD);

};

extern RecordClient *recordClient;

class SessionClient : public zTCPBufferClient,public MessageQueue
{

  public:
    /**
    * \brief 构造函数
    * \param  name 名称
    * \param  ip   地址
    * \param  port 端口
    */
    SessionClient(
        const std::string &name,
        const std::string &ip,
        const WORD port)
      : zTCPBufferClient(name,ip,port) {};

    bool connectToSessionServer();
    void run();

    void requestFriendDegree(SceneUser *pUser);
    bool msgParse(const Cmd::t_NullCmd *pNullCmd,const DWORD nCmdLen);
    bool doGmCmd(const Cmd::t_NullCmd *pNullCmd,const DWORD nCmdLen);
    bool doAuctionCmd(const Cmd::Session::t_AuctionCmd *,const DWORD);
    bool doCartoonCmd(const Cmd::Session::t_CartoonCmd *,const DWORD);
    bool cmdMsgParse(const Cmd::t_NullCmd *,const DWORD);
    bool cmdMsgParse_Country(const Cmd::t_NullCmd*,const DWORD);
    bool cmdMsgParse_Dare(const Cmd::t_NullCmd*,const DWORD);
    bool cmdMsgParse_Recommend(const Cmd::t_NullCmd*,const DWORD);
    bool cmdMsgParse_Other(const Cmd::t_NullCmd*,const DWORD);
    bool cmdMsgParse_Temp(const Cmd::t_NullCmd*,const DWORD);
    bool cmdMsgParse_Army(const Cmd::t_NullCmd* pNullCmd,const DWORD nCmdLen);
    bool cmdMsgParse_Sept(const Cmd::t_NullCmd*,const DWORD);
    bool cmdMsgParse_Union(const Cmd::t_NullCmd*,const DWORD);
    bool cmdMsgParse_Gem(const Cmd::t_NullCmd* pNullCmd,const DWORD nCmdLen);
	//sky session 战场竞技场副本消息处理函数
	bool cmdMsgParse_Sports(const Cmd::t_NullCmd* pNullCmd,const DWORD nCmdLen);
	//sky end
};

/// 声明
extern SessionClient *sessionClient;

/**
 * \brief NPC买卖
 *
 */
class NpcTrade
{

  public:

    enum
    {
      NPC_BUY_OBJECT    = 1, ///买
      NPC_SELL_OBJECT    = 2, ///卖
      NPC_REPAIR_OBJECT  = 4, ///修理
      NPC_MAKE_OBJECT    = 8, ///打造
      NPC_UPDATE_OBJECT  = 16, ///改造
      NPC_MERGE_OBJECT  = 32, ///合成
      NPC_ENCHANCE_OBJECT = 64,//镶嵌
      NPC_MERGE_SOUL_OBJECT = 128,//魂魄合成
      NPC_HOLE_OBJECT = 256,//打孔
      NPC_STORE_OBJECT = 512,//仓库
      NPC_DECOMPOSE_OBJECT = 1024,//分解
	  NPC_ZSMAKE_OBJECT = 2048,
	  NPC_ZSUPDATE_OBJECT = 4096,

    };

    struct NpcItem
    {
      DWORD id;          ///物品编号
      WORD  kind;          ///物品类型
      WORD  lowLevel;        ///最低等级
      WORD  level;        ///最高等级
      WORD  itemlevel;      ///购买物品的等级
      WORD  action;        ///动作类型
      NpcItem()
      {
        id = 0;
        kind = 0;
        lowLevel = 0;
        level = 0;
        itemlevel = 0;
        action = 0;
      }
    };

    ~NpcTrade()
    {
      final();
    }

    /**
     * \brief 得到唯一实例
     *
     *
     * \return npc买卖系统
     */
    static NpcTrade &getInstance()
    {
      if (NULL == instance)
        instance = new NpcTrade();

      return *instance;
    }

    /**
     * \brief 卸载唯一实例
     *
     */
    static void delInstance()
    {
      SAFE_DELETE(instance);
    }

    bool init();
    bool getNpcMenu(const DWORD npcid,char *menuTxt);
    bool verifyNpcAction(const DWORD npcid,const NpcItem &item);

  private:

    NpcTrade() {};

    void final();

    static NpcTrade *instance;

    typedef hash_multimap<DWORD,NpcItem> NpcItemMultiMap;

    /**
     * \brief npc对话框
     *
     */
    struct NpcDialog
    {
      DWORD npcid;      ///Npc编号
      char menu[6144];    ///菜单内容
      NpcItemMultiMap items;  ///物品动作
      NpcDialog()
      {
        npcid = 0;
        bzero(menu,sizeof(menu));
      }
      NpcDialog(const NpcDialog& nd)
      {
        npcid = nd.npcid;
        bcopy((void *)nd.menu,menu,sizeof(menu),sizeof(menu));
        for(NpcItemMultiMap::const_iterator it = nd.items.begin(); it != nd.items.end(); it++)
        {
          items.insert(*it);
        }
      }
    };

    typedef hash_multimap<DWORD,NpcDialog> NpcDialogMultiMap;

    NpcDialogMultiMap dialogs;
    zRWLock rwlock;

};

/**
 * \brief 镖车
 *
 * 该类封装了对特殊NPC镖车的抽象
 *
 */
class GuardNpc : public ScenePet
{
public:  
  GuardNpc(Scene *scene,zNpcB *npc,const t_NpcDefine *define,const SceneNpcType type,const SceneEntryType entrytype = SceneEntry_NPC,zNpcB *abase=NULL);
  
  ~GuardNpc();
  
  void owner(SceneUser* user);  
  SceneUser* owner();
  
  void gold(int money);
  int gold() const;
  
  void exp(int experience);
  int exp() const;

  void dest(const zPos& pos);
  void map(const std::string& name);
  
  void reset();
  void check();

  void on_death(SceneEntryPk*);

  //bool masterIsAlive;
  bool canMove();
  bool moveToMaster();
  DWORD save(BYTE * dest);

  bool isSeptGuard;
private:
  void on_reached();
  
  //SceneUser* _owner;
  std::string _name;
  std::string _map;
      
  int _exp;
  DWORD _gold;
  zPos _dest;
  
  int _status;
  
  zRTime _time;  

  DWORD getRobGold();
  //char ownerName[MAX_NAMESIZE];
};

#define SHELLITEM_IN_SEPT               20
#define SHELLITEM_IN_SEPT_TOP   20
#define SHELLITEM_IN_COUNTRY    80
#define SHELLITEM_IN_FRIEND             20
#define SHELLITEM_IN_TEAM               10
#define SHELLITEM_IN_UNION              50
#define SHELLITEM_IN_PRIVATE    10
#define SHELLITEM_IN_NINE               10
#define SHELLITEM_IN_PERSON    10
#define SHELLITEM_IN_ZONE  10

/**
 * \brief 聊天频道
 *
 */
class Channel:public zEntry
{
        private:
                std::vector<zEntryC> userlist;
                zEntry creater;
        public:
                static bool sendNine(SceneUser *pUser,const Cmd::stChannelChatUserCmd *rev,DWORD cmdLen);
                static bool sendNine(SceneUser *pUser,const char *pattern,...);
                static bool sendNine(const SceneNpc *pNpc,const char *pattern,...);
                static bool sendPrivate(SceneUser *pUser,const Cmd::stChannelChatUserCmd *rev,DWORD cmdLen);
                static bool sendTeam(DWORD teamid,const Cmd::stChannelChatUserCmd *rev,DWORD cmdLen);
                static bool sendCountry(SceneUser *pUser,const Cmd::stChannelChatUserCmd *rev,DWORD cmdLen);
                bool sendToAll(SceneUser *pUser,const Cmd::stChannelChatUserCmd *rev,DWORD cmdLen);
                static bool sendMapInfo(DWORD mapID,DWORD infoType,const char *pattern,...);
                static bool sendMapInfo(Scene * s,DWORD infoType,const char *pattern,...);
                static bool sendCmdToMap(DWORD mapID,const void *cmd,int len);
                static bool sendCmdToMap(Scene * scene,const void *cmd,int len);

                static bool sendNine(SceneUser *pUser,const char *content);
                static bool sendPrivate(SceneUser *pUser,const char *toName,const char *pattern,...);
                static bool sendPrivate(const char *src_name,const char *toName,const char *pattern,...);
                static bool sendSys(SceneUser *pUser,int type,const char *pattern,...);
                static bool sendMoney(SceneUser *pUser,int type,DWORD money,const char *pattern,...);
                static bool sendGold(SceneUser *pUser,int type,DWORD gold,const char *pattern,...);
                static bool sendSys(DWORD dwUserTempID,int type,const char *pattern,...);
                static bool sendTeam(DWORD teamid,const char *pattern,...);
                static bool sendCountry(SceneUser *pUser,const char *pattern,...);
                static bool sendCountryInfo(DWORD countryID,DWORD infoType,const char *pattern,...);
                static bool sendAllInfo(DWORD infoType,const char *pattern,...);

    static bool doPreCheck(SceneUser *pUser,Cmd::stChannelChatUserCmd *cchat,DWORD cmdLen);
                bool sendToAll(SceneUser *pUser,const char *pattern,...);
                bool sendCmdToAll(const void *cmd,int len);
                Channel(SceneUser *pUser);
                bool remove(const char *name);
                bool add(const char *name);
                bool add(SceneUser *pUser);
                WORD has(const char *name);
};

/**
 * \brief 频道管理器
 *
 */
class ChannelM:public zEntryManager< zEntryTempID,zEntryName >
{
        private:
                static ChannelM * cm;
                zUniqueDWORDID *channelUniqeID;
                zMutex mlock;

                ChannelM();
                ~ChannelM();

                bool getUniqeID(DWORD &tempid);
                void putUniqeID(const DWORD &tempid);

    public:
                static ChannelM &getMe();
                static void destroyMe();
                bool add(Channel *channel);
                void remove(DWORD dwChannelID);
                Channel *get(DWORD dwChannelID);
                void removeUser(const char *name);
};

/// 超级GM的id,只有1个超级GM
#define SUPER_GM_ID 1

struct Gm
{       
        enum
    {       
      none_mode =   0x00,
      normal_mode =   0x01,
      gm_mode =    0x02,
      captain_mode =   0x04,
      super_mode =   0x08,
      debug_mode =   0x10,
      all_mode =   0x1f,/// all_mode
      //admin_mode =   0x10,
      //magic_mode =   0x20
    };
  
        const char *cmd;
        bool (*parse)( SceneUser *pUser,const char *para);
  BYTE priv;
        const char *desc;
  
        static void sendLog(SceneUser *pUser,const char *cmd,const char * content); 

        static void exec(SceneUser *pUser,char *cmd); 
        static bool fetch(SceneUser *pUser,const char *para);
        static bool getgive(SceneUser *pUser,const char *para);
        static bool summon(SceneUser *pUser,const char *para);
        static bool help(SceneUser *pUser,const char *para);
        static bool levelup(SceneUser *pUser,const char *para);
        static bool goTo(SceneUser *pUser,const char *para);
        static bool goTo_Gm(SceneUser *pUser,const char *para);
        static bool gomap(SceneUser *pUser,const char *para);
        static bool gomap_Gm(SceneUser *pUser,const char *para);
        static bool upgradeSkill(SceneUser *pUser,const char *para);
        static bool abandon(SceneUser* pUser,const char* para);
        static bool finduser(SceneUser* pUser,const char* para);
        static bool gotouser(SceneUser* pUser,const char* para);
        static bool catchuser(SceneUser* pUser,const char* para);
  static bool bczone(SceneUser* pUser,const char* para);
  static bool bcworld(SceneUser* pUser,const char* para);
  static bool bcwgamemsg(SceneUser *pUser,const char *para);
  static bool leechdom(SceneUser* pUser,const char* para);
  static bool value(SceneUser* pUser,const char* para);
  static bool team(SceneUser* pLeader,const char* para);
  static bool throwobject(SceneUser* pLeader,const char* para);
  static bool querypoint(SceneUser* pLeader,const char* para);
  static bool redeemgold(SceneUser* pLeader,const char* para);
  static bool querygold(SceneUser* pLeader,const char* para);
  static bool redeemmonthcard(SceneUser* pLeader,const char* para);
  static bool goldsystem(SceneUser* pLeader,const char* para);
  static bool stockconsign(SceneUser* pLeader,const char* para);
  static bool stocktransfer(SceneUser* pLeader,const char* para);
  static bool stockuser(SceneUser* pLeader,const char* para);
  static bool stocksystem(SceneUser* pLeader,const char* para);
  static bool givestock(SceneUser* pLeader,const char* para);
  static bool givemoney(SceneUser* pLeader,const char* para);
  static bool givegold(SceneUser* pLeader,const char* para);
  static bool backoff(SceneUser* pUser,const char* para);
  static bool goodness(SceneUser* pUser,const char* para);
  static bool kick(SceneUser* pUser,const char* para);
  static bool kickGateUser(SceneUser* pUser,const char* para);
  static bool donttalk(SceneUser* pUser,const char* para);
  static bool talk(SceneUser* pUser,const char* para);
  static bool setPriv(SceneUser* pUser,const char* para);
  static bool hideMe(SceneUser* pUser,const char* para);
  static bool showMe(SceneUser* pUser,const char* para);
  static bool goHome(SceneUser* pUser,const char* para);
  static bool loadMap(SceneUser* pUser,const char* para);
  static bool unloadMap(SceneUser* pUser,const char* para);
  static bool createRush(SceneUser* pUser,const char* para);
  static bool createQuiz(SceneUser* pUser,const char* para);
  static bool clearWorld(SceneUser* pUser,const char* para);
  static bool clearSeptUnion(SceneUser* pUser,const char* para);
  static bool createFamily(SceneUser* pUser,const char* para);
  static bool viewCountryDare(SceneUser* pUser,const char* para);
  static bool loadProcess(SceneUser *pUser,const char * para);
  static bool loadGift(SceneUser *pUser,const char * para);
  static bool clearArea(SceneUser *pUser,const char *para);
  static bool embar(SceneUser *pUser,const char *para);
  static bool closenpc(SceneUser *pUser,const char * para);
  static bool countrypower(SceneUser *pUser,const char * para);
  static bool refreshGeneral(SceneUser *pUser,const char * para);

  static bool createUnion(SceneUser* pUser,const char* para);
  static bool createSchool(SceneUser* pUser,const char* para);
  static bool showEntries(SceneUser* pUser,const char* para);
  static bool getsize(SceneUser *pUser,const char *para);
  static bool setPetAI(SceneUser *pUser,const char *para);
  static bool setPetAIF(SceneUser *pUser,const char *para);
  static bool addExp(SceneUser *pUser,const char *para);
  static bool reloadScript(SceneUser *pUser,const char *para);
  static bool countServerUser(SceneUser* pUser,const char* para);
  static bool shutdown(SceneUser* pUser,const char* para);
  static bool systime(SceneUser* pUser,const char* para);
  static bool usleep(SceneUser* pUser,const char* para);
  static bool qAccount(SceneUser *pUser,const char *para);
  static bool setQuest(SceneUser* pUser,const char* para);
  static bool checkQuest(SceneUser* pUser,const char* para);
  static bool debugVote(SceneUser* pUser,const char* para);
  static bool debugGem(SceneUser* pUser,const char* para);
  static bool addSeptExp(SceneUser* pUser,const char* para);

  static bool setRepute(SceneUser* pUser,const char* para);
  static bool setSeptLevel(SceneUser* pUser,const char* para);
  static bool setAllyFriendDegree(SceneUser* pUser,const char* para);
  static bool changeCountry(SceneUser* pUser,const char* para);
  static bool debugCityDare(SceneUser* pUser,const char* para);
  static bool debugEmperorDare(SceneUser* pUser,const char* para);
  static bool setService(SceneUser* pUser,const char* para);
  static bool setTire(SceneUser *pUser,const char * para);
  static bool enableLogin(SceneUser *pUser,const char * para);
  static bool enableRegister(SceneUser *pUser,const char * para);
  static bool checkCountryInfo(SceneUser *pUser,const char * para);
  static bool version(SceneUser *pUser,const char * para);
  
  /**
   * \brief 技能测试指令
   */
  static bool skill(SceneUser *pUser,const char *para);
  static bool lockValue(SceneUser *pUser,const char *para);
  static bool getvalue(SceneUser *pUser,const char *para);
  static bool setvalue(SceneUser* pUser,const char* para);
  static bool god(SceneUser *pUser,const char *para);
  static bool killer(SceneUser *pUser,const char *para);
  static bool newzone(SceneUser *pUser,const char *para);
  static bool normal(SceneUser *pUser,const char *para);
  static bool svote(SceneUser *pUser,const char *para);
  static bool uvote(SceneUser *pUser,const char *para);
  static bool tong(SceneUser *pUser,const char *para);
  static bool callPet(SceneUser *pUser,const char *para);
  static bool showMaps(SceneUser* pUser,const char* para);
  static bool showPets(SceneUser* pUser,const char* para);
  static bool killPets(SceneUser* pUser,const char* para);
  static bool showSpecialNpc(SceneUser* pUser,const char* para);
  static bool bank(SceneUser* pUser,const char* para);
  static bool setBlock(SceneUser* pUser,const char* para);
  static bool checkBlock(SceneUser* pUser,const char* para);
  static bool npcDare(SceneUser *pUser,const char * para);
  static bool clearPoint(SceneUser *pUser,const char * para);
  static bool clearSkillPoint(SceneUser *pUser,const char * para);
  static bool studySkill(SceneUser *pUser,const char * para);
  static bool clearColddown(SceneUser *pUser,const char * para);
  static bool queryincmap(SceneUser *pUser,const char * para);
  static bool showAddExp(SceneUser *pUser,const char * para);
  static bool changeHorse(SceneUser *pUser,const char * para);
  static bool contribute(SceneUser *pUser,const char * para);
  static bool clearSkill(SceneUser *pUser,const char * para);
};

extern bool sendCmdByID(DWORD id,const void *cmd,int len);
extern bool sendCmdByTempID(DWORD tempid,const void *cmd,int len);
extern bool sendCmdByName(char * name,const void *cmd,int len);

class CartoonPet : public ScenePet
{
  private:
    DWORD cartoonID;
    Cmd::t_CartoonData cartoonData;

    BYTE expRate;//释放经验的速率
    Timer _5_sec;
    DWORD _5_sec_count;

    void releaseExp();
  public:
    void releaseExp(DWORD);

    CartoonPet(Scene *scene,zNpcB *npc,const t_NpcDefine *define,const SceneNpcType type,const SceneEntryType entrytype = SceneEntry_NPC,zNpcB *abase=NULL);

    ~CartoonPet(){};

    Cmd::t_CartoonData& getCartoonData();
    void setCartoonData(Cmd::t_CartoonData&);

    void setMaster(SceneEntryPk *);
    void setMaster(DWORD,DWORD);
    void setCartoonID(DWORD);
    DWORD getCartoonID(){return cartoonID;}
    void setExpRate(BYTE);
    void recoverSp(DWORD);
    void setName(char *);

    void putAway(Cmd::Session::saveType saveType);
    void drawExp();
    bool normalAction();
    void sendData();
    void sendHpExp();
    bool addExp(DWORD);
    void levelUp();

    DWORD getLevel() const;
    bool isAdopted();

    void save(Cmd::Session::saveType saveType);
    DWORD save(BYTE * dest);

    void delMyself();
};

class Dice
{
  public:
    Dice(SceneUser * u1,SceneUser * u2,DWORD money);
    ~Dice(){};

    enum DiceState
    {
      DICE_STATE_CREATE,//未初始化，可以聊天
      DICE_STATE_ROLLING,//色子在转，等待停止消息
      DICE_STATE_END,//一局开始之前等待双方准备
      DICE_STATE_DEL//等待删除
    };

    bool init();

    bool sendCmdToAll(const void *cmd,const DWORD len);
    bool sendAllInfo(const char *pattern,...);

    void endGame(SceneUser * user=0);
    bool rotate(DWORD id);
    bool setReady(SceneUser * user);

    DWORD getMoney();
    DWORD getTheOtherTempID(DWORD id);
    bool judge();

    DiceState getState();
    bool timer(DWORD time,SceneUser * u);
  private:
    DWORD startTime;

    DWORD round;//n周目
    DWORD money;
    DiceState gameState;

    DWORD tempid1,tempid2;
    DWORD value1,value2;
    bool continue1,continue2;
    char name1[MAX_NAMESIZE],name2[MAX_NAMESIZE];
};

/**
 * \brief 新手装备结构
 *
 */
struct InitObject
{
  ///
  DWORD id;
  char name[MAX_NAMESIZE+1];
  DWORD localeID;
  WORD  num;
  WORD  profession;

  InitObject()
  {
    id = 0;
    bzero(name,sizeof(name));
    localeID = 0;
    num = 0;
    profession = 0;
  }

  /**
   * \brief 构造函数
   *
   *
   * \param object: 新手装备
   */
  InitObject(const InitObject &object)
  {
    id = object.id;
    bcopy((void *)object.name,name,sizeof(name),sizeof(name));
    localeID = object.localeID;
    num = object.num;
    profession = object.profession;
  }

};

typedef std::vector<InitObject> InitObjectVector;

/**
 * \brief 新手信息
 *
 */
class CharInitInfo
{

  public:

    ~CharInitInfo()
    {
      final();
    }

    /**
     * \brief 得到唯一实例
     *
     *
     * \return 唯一实例
     */
    static CharInitInfo &getInstance()
    {
      if (NULL == instance)
        instance = new CharInitInfo();

      return *instance;
    }

    /**
     * \brief 删除唯一实例
     *
     *
     */
    static void delInstance()
    {
      SAFE_DELETE(instance);
    }

    bool init();
    void get(const WORD profession,InitObjectVector &objs);
    void final();

  private:

    static CharInitInfo *instance;

    CharInitInfo() {};

    typedef hash_multimap<WORD,InitObject> ObjectsContainer;
    zRWLock rwlock;
    ObjectsContainer objects;

};

namespace Cmd{
struct stPropertyUserCmd;
struct stFoundItemPropertyUserCmd;
}

class zObjectB;

/**
 * \brief 物品改造基类
 *
 * 封装了几个常用的操作
 *
 */  
class Base
{
public:  
  enum ReuildType{
    COMPOSE = 1,
    MAKE = 2,
    UPGRADE = 3,
    HOLE = 4,
    ENCHANCE = 5,
    DECOMPOSE = 6,
  };
  
  static bool check_npc(SceneUser& user,zObjectB* base,int action);
  
  static bool check_space(SceneUser& user,DWORD w,DWORD h);

  static bool remove_object(SceneUser& user,zObject* ob);
  
  static bool add_object(SceneUser& user,zObject* ob,bool add = true);

  static bool response(SceneUser& user,int status,BYTE type);
    
  static void refresh_pack(SceneUser& user,ObjectPack* pack);
};

/**
 * \brief 孔
 *
 * 封装了对与孔相关的处理函数
 *
 */  
class Hole
{
public:
  enum {
    INVALID_INDEX = 4,
    INVALID_NUM = 5,
  };
  
  
  static bool can_hole(zObject * ob);
  
  static int get_empty_hole(zObject* ob);
  
  static int get_hole_num(zObject* ob);

  static int add_hole_num(zObject* ob,int num);
  
  static bool add_hole(zObject* ob,int index);

  static bool put_hole(zObject* ob,int index,zObject* gam); 

  //sky 激活孔
  static bool Activated_hole(zObject* ob, int index);

};

/**
 * \brief 魂魄石
 *
 * 封装了对魂魄石的处理函数,包括镶嵌及合成等
 *
 */  
class SoulStone
{
public:
  static zObject* compose(SceneUser& user,zObject* first,zObject* second,int odds);

  static bool enchance(SceneUser& user,zObject* dest,zObject* src);

  static int id(DWORD trait);
  
  static bool assign(zObject* ob,int monster);
  
private:
  static bool do_enchance(zObject* dest,zObject* src);
  
  static bool do_compose(zObject* first,zObject* second,zObject* dest);
  
  /**     
   * \brief 魂魄属性计算
   *
   * \param first: 第一块魂魄石属性
   * \param second: 第二块魂魄石属性
   * \param result: 计算结果
   * \param level:魂魄石等级
   * \return 无
   */  
  template<typename T>
  static void additive(T first,T second,T& result,int level)
  {
    if (first && second) {
      //result = std::max(first,second) + 0.1*std::min(first,second)  
      T max = first,min = second,grade = level & 0xff;
      if (max < min) {
        max = second;
        min = first;
        grade = level >> 8;
      }
      
      result =  max + static_cast<T>(min*0.1*grade);  
    }else {
      result = first + second;
    }
    
  }

  static const int _ids[];
};

/**
 * \brief 升级
 *
 * 实现了物品升级功能
 *
 */  
class Upgrade
{
public:
  static bool upgrade(SceneUser& user,zObject* ob,int extra_odds);

private:  
  static bool do_upgrade(zObject* ob,zUpgradeObjectB* base);    
  static bool do_downgrade(zObject* ob,zUpgradeObjectB* base);    

};

/**
 * \brief 物品分解
 *
 * 实现物品分解功能
 *
 */
class Decompose
{
public:  
  /**     
   * \brief 构造函数
   *
   * 初始化相关变量
   *
   * param ob : 待分解物品
   *
   */   
  Decompose(zObject* ob) : _ob(ob)
  { }
  
  bool bonus_items(SceneUser& user);
  bool bonus_exp(SceneUser& user);
  bool remove_from(SceneUser& user);  
  int gold() const;
    
private:
  
  int chance() const;
  int index() const;
  
  zObject* _ob;
  
  const static int _odds[];
  const static int _items[];
};

#define COMPUTE_R(x) additive(ob->data.x,bob->x);
#define COMPUTE_L(x) additive(ob->data.x,bob->x,property);

#define BONUS_SKILL std::vector<skillbonus>::iterator it = bob->skill.begin();  \
    bool must = false; \
    for ( ; it!=bob->skill.end(); ++it) { \
      int odds = odds_of_property(it->odds,property); \
      if (selectByTenTh(odds) || must) { \
        int i = 0; \
        while ( i <3) {\
          if (ob->data.skill[i].id == 0 || ob->data.skill[i].id==it->id) {\
            ob->data.skill[i].id = it->id; \
            ob->data.skill[i].point += it->level; \
            break;\
          }else {\
            ++i;\
          }\
          if (i>2 &&must) break;\
        }\
      } \
    }
//戒指,项链
#define BONUS_SKILL_RING {\
  if (!bob->skill.empty())\
  {\
    int index = randBetween(0,bob->skill.size()-1 );\
        int i = 0; \
        while ( i <3) {\
          if (ob->data.skill[i].id == 0 || ob->data.skill[i].id==bob->skill[index].id) {\
            ob->data.skill[i].id = bob->skill[index].id; \
            ob->data.skill[i].point += bob->skill[index].level; \
            break;\
          }else {\
            ++i;\
          }\
        }\
    }\
  }

#define BONUS_SKILLS int odds = odds_of_property(bob->skills.odds,property); \
    if (selectByTenTh(odds)) { \
      ob->data.skills.id = bob->skills.id; \
      ob->data.skills.point = bob->skills.level; \
    }

/**
 * \brief 打造
 *
 * 实现了物品打造功能
 *
 */  
class EquipMaker
{
  public:
    EquipMaker(SceneUser* user);

    /**     
     * \brief 析构函数
     *
     */     
    ~EquipMaker() { }

    bool check_skill(SceneUser& user,zObjectB* ob);

    bool add_skill(SceneUser& user,zObjectB* ob);
    bool add_exp(SceneUser& user,DWORD exp);

    bool check_material(zObjectB* ob,const std::map<DWORD,DWORD>& list,bool is_resource = false);
	bool EquipMaker::check_material1(zObjectB* ob,const std::map<DWORD,DWORD>& list);

    //  void pre_level_of_material(zObjectB* base);
    void pre_level_of_material(int id,int level);

    bool level_of_material(DWORD id,DWORD num,DWORD level,zObjectB* base);

    bool is_odds_gem(DWORD kind,DWORD id);

    zObject* make_material(zObjectB* base);

    zObject* make(SceneUser* user,zObjectB* base, BYTE kind, int flag = 0);

    void assign(SceneUser* user,zObject* ob,zObjectB* base,BYTE kind);

	//sky 新的极品计算函数
	void NewAssign(SceneUser* user,zObject* ob,zObjectB* base,DWORD npc_mul);

    void fix(zObject* ob);

    void bonus_hole(zObject* ob);

  private:
    struct Odds
    {
      int per;
      int luck;
      double material_level;
      int skill_level;
      int odds;    
      int sleight;
      int odds_gem;
      Odds() : per(0),luck(0),material_level(0),skill_level(0),odds(0),sleight(0),odds_gem(0)
      { }
    };

    Odds _odds;
    double _current;
    double _base;

    bool _make;

    int _need;
    int _1_id;
    int _1_level;
    int _2_id;
    int _2_level;

    int odds_of_white(const zObjectB* ob);
    int odds_of_blue(const zObjectB* ob);
    int odds_of_gold(const zObjectB* ob);
    int odds_of_holy(int object);
    int odds_of_property(int object,int property);

    template <typename T>
      void fix_kind(T* bob,zObject* ob)
      {
        switch (ob->base->kind)
        {
          case ItemType_ClothBody ://101代表布质加生命类服装
            additivePercent(ob->data.maxhp,bob->maxsp);
            break;
          case ItemType_FellBody :      //102代表皮甲加魔防类服装
            additivePercent(ob->data.mdefence,bob->maxsp);  
            break;
          case ItemType_MetalBody:  //103代表金属铠甲加物防类服装
          case ItemType_Shield:   //112代表盾牌类
            additivePercent(ob->data.pdefence,bob->maxsp);      
            break;
          case ItemType_Blade:        //104代表武术刀类武器
          case ItemType_Sword:        //105代表武术剑类武器
          case ItemType_Axe:             //106代表武术斧类武器
          case ItemType_Hammer:          //107代表武术斧类武器
          case ItemType_Crossbow:          //109代表箭术弓类武器
            additivePercent(ob->data.pdamage,bob->maxsp);    
            additivePercent(ob->data.maxpdamage,bob->maxsp);    
            break;
          case ItemType_Staff:        //108代表法术杖类武器
          case ItemType_Stick:          //111代表召唤棍类武器
          case ItemType_Fan:             //110代表美女扇类
            additivePercent(ob->data.mdamage,bob->maxsp);    
            additivePercent(ob->data.maxmdamage,bob->maxsp);    
            break;
		  /*sky 新增板和皮类型防具支持*/
		  case ItemType_Helm:    //113代表角色头盔布
		  case ItemType_Caestus:  //114代表角色腰带布
		  case ItemType_Cuff:    //115代表角色护腕布
		  case ItemType_Shoes:    //116代表角色鞋子布
		  case ItemType_Helm_Paper: //头盔皮
		  case ItemType_Helm_Plate: //头盔板
		  case ItemType_Cuff_Paper: //护腕皮
		  case ItemType_Cuff_Plate: //护腕板
		  case ItemType_Caestus_Paper: //腰带皮
		  case ItemType_Caestus_Plate: //腰带板
		  case ItemType_Shoes_Paper: //靴子皮
		  case ItemType_Shoes_Plate: //靴子板
		  //sky 新增肩膀 手套 裤子类
		  case tyItemType_Shoulder:
		  case tyItemType_Gloves:
		  case tyItemType_Pants:
		  case ItemType_Shoulder_Paper:
		  case ItemType_Gloves_Paper:
		  case ItemType_Pants_Paper:
		  case ItemType_Shoulder_Plate:
		  case ItemType_Gloves_Plate:
		  case ItemType_Pants_Plate:
            if (randBetween(0,1)) {
              additivePercent(ob->data.pdefence,bob->maxsp);    
            }else {
              additivePercent(ob->data.mdefence,bob->maxsp);        
            }
            break;
          case ItemType_Necklace:  //117代表角色项链类
          case ItemType_Fing:    //118代表角色戒指类
            if (ob->data.pdamage || ob->data.maxpdamage) {
              additivePercent(ob->data.pdamage,bob->maxsp);    
              additivePercent(ob->data.maxpdamage,bob->maxsp);    
            }
            if (ob->data.mdamage || ob->data.maxmdamage) {
              additivePercent(ob->data.mdamage,bob->maxsp);    
              additivePercent(ob->data.maxmdamage,bob->maxsp);    
            }
            break;
        }

      }


    /**     
     * \brief 属性取最大值
     *
     * \param ret: 计算结果
     * \param lv: 属性取值范围
     * \return 无
     */  
      /*template<class T>
      bool max(T& ret,const luckRangeValue &rv)
      {
        if (selectByTenTh(rv.per) )  {
          ret += rv.data.max;

        }
        return false;
      }*/
    /**     
     * \brief 生成特殊装备
     *
     * \param bob: 对应装备基本表
     * \param ob: 打造物品
     * \param kind: 装备类型
     * \return 当前总是返回true
     */  

    template <typename T>
      bool assign_color(T* bob,zObject* ob,int kind,int props = 0,zObjectB *base=NULL,bool drop=false)
      {
        char tmp[MAX_NAMESIZE];
        _snprintf_s(tmp,MAX_NAMESIZE,"%s%s%s",bob->prefix,bob->joint,ob->data.strName);
        strncpy_s(ob->data.strName,tmp,MAX_NAMESIZE);

        int property = 1;

        if (props >= 17)
        {
          ++property;
          //ob->data.fivepoint += randBetween(bob->five.data.min,bob->five.data.max);
          if (/*ob->data.fivepoint &&// */ ob->data.fivetype == FIVE_NONE) 
          {
            ob->data.fivetype = randBetween(0,4);
          }
        }
        else //if (additive(ob->data.fivepoint,bob->five,property)) 
        {
          //ob->data.fivepoint -= bob->five.sleightValue;
          if (/*ob->data.fivepoint && // */ob->data.fivetype == FIVE_NONE && selectByTenTh(bob->five.per)) 
          {
            ob->data.fivetype = randBetween(0,4);
          }
        }


        COMPUTE_R( pdamage )    // 最小物攻
          COMPUTE_R( maxpdamage )    // 最大物攻
          COMPUTE_R( mdamage )      // 最小魔攻
          COMPUTE_R( maxmdamage )    // 最大魔攻
          COMPUTE_R( pdefence )      // 物防
          COMPUTE_R( mdefence )      // 魔防

          if (props) {
            int index = randBetween(0,4);
            if (index!=5) {

              //现在不用随机了,直接取值
              additivePercent(*ob->_p1[index],bob->_p1[index]);
            }else {
              fix_kind(bob,ob);
            }
          }else {
            COMPUTE_L( str )      // 力量
              COMPUTE_L( inte )      // 智力
              COMPUTE_L( dex )      // 敏捷
              COMPUTE_L( spi )      // 精神
              COMPUTE_L( con )        // 体质
          }    

        COMPUTE_L( maxhp )    // 最大生命值
          COMPUTE_L( maxmp )    // 最大法术值
          //    COMPUTE_L( maxsp )    // 最大体力值

          COMPUTE_L( mvspeed )    // 移动速度
          COMPUTE_L( hpr )      // 生命值恢复
          COMPUTE_L( mpr )      // 法术值恢复
          COMPUTE_L( spr )      // 体力值恢复
          COMPUTE_L( akspeed )    // 攻击速度

          if (props) {
            for (int i=0; i<=17; ++i){
              if (props == 17){
                if (i == 0 || i == 2)
                {
                  if (bob->_p2[i].per)
                    *ob->_p2[i] += bob->_p2[i].data.max;
                }
                else if (i == 1 || i == 3)
                {
                  if (bob->_p2[i].per)
                  {
                    int temp = randBetween(((bob->_p2[i].data.max - bob->_p2[i].data.min)/2 + bob->_p2[i].data.min + 1),bob->_p2[i].data.max);
                    *ob->_p2[i] += temp;
                  }
                }
                else
                {
                    max(*ob->_p2[i],bob->_p2[i]);
                }
              }else if (props == 18){
                if (i == 0 || i == 2)
                {
                  if (bob->_p2[i].per)
                    *ob->_p2[i] += 10;
                }
                else if (i == 1 || i == 3)
                {
                  if (bob->_p2[i].per)
                    *ob->_p2[i] += bob->_p2[i].data.max;
                }
                else
                {
                    max(*ob->_p2[i],bob->_p2[i]);
                }
              }else{
                additive(*ob->_p2[i],bob->_p2[i]);
              }
            }
            /*
               std::vector<int> list;
               for (int i=0; i<=17; ++i) list.push_back(i);
            //每个都需要随机
            //int geted = props;
            // */
            /*
               if (props == 5){
               if (list.size() >=4){
               additive(*ob->_p2[list[3]],bob->_p2[list[3]]);
               }
               if (list.size() >=8){
               additive(*ob->_p2[list[7]],bob->_p2[list[7]]);
               }
               }else{
            // */
            //while (/*geted -- > 0 && */list.size() > 0) {
            /*
               int index = randBetween(0,list.size()-1 );
               int p = list[index];
            //取最大值
            if (props == 17){
            max(*ob->_p2[p],bob->_p2[p]);
            }else{
            additive(*ob->_p2[p],bob->_p2[p]);
            }
            std::vector<int>::iterator it = list.begin();
            list.erase(it+index);
            // */
            //}
            //}

          }else {
            COMPUTE_L( pdam )    // 增加物理攻击力
              COMPUTE_L( mdam )    // 增加魔法攻击力
              COMPUTE_L( pdef )    // 增加物理防御力
              COMPUTE_L( mdef )    // 增加魔法防御力
              COMPUTE_L( atrating )    // 命中率
              COMPUTE_L( akdodge )    // 闪避率

              COMPUTE_L( poisondef )  // 抗毒增加
              COMPUTE_L( lulldef )    // 抗麻痹增加
              COMPUTE_L( reeldef )    // 抗眩晕增加
              COMPUTE_L( evildef )    // 抗噬魔增加
              COMPUTE_L( bitedef )    // 抗噬力增加
              COMPUTE_L( chaosdef )  // 抗混乱增加
              COMPUTE_L( colddef )    // 抗冰冻增加
              COMPUTE_L( petrifydef )    // 抗石化增加
              COMPUTE_L( blinddef )    // 抗失明增加
              COMPUTE_L( stabledef )    // 抗定身增加
              COMPUTE_L( slowdef )    // 抗减速增加
              COMPUTE_L( luredef )    // 抗诱惑增加
          }

        if (!ob->data.durpoint) {
          if (additive(ob->data.durpoint,bob->resumedur,property)) {
            ob->data.durpoint -= bob->resumedur.sleightValue;
          }
          if (ob->data.durpoint) ob->data.dursecond = bob->resumedur.sleightValue;

        }

        COMPUTE_L( bang )       //重击
          //ob->data.bang += bob->bang;
          //戒指项链至少一个
          if (ob->base->kind == ItemType_Fing || ob->base->kind == ItemType_Necklace)
          {
            BONUS_SKILL_RING
          }
        BONUS_SKILL
          BONUS_SKILLS

          if (props) 
            ob->data.kind |= 2;//有色装备
          else 
            ob->data.kind |= kind;//有色装备

        return true;
      }

    bool assign_holy(zObject* ob,int holy);

public:
    bool assign_set(zObject* ob);
private:
    /**     
     * \brief 属性计算
     *
     * \param ret: 计算结果
     * \param lv: 属性取值范围
     * \return 无
     */  
    template <typename T>
      void additive(T& ret,const rangeValue &rv)
      {
        ret += randBetween(rv.min,rv.max);
      }

    /**     
     * \brief 神圣属性计算
     *
     * \param ret: 计算结果
     * \param lv: 属性取值范围
     * \param property: 物品当前属性数目
     * \return 无
     */  
    template <typename T>
      bool additive(T& ret,const luckRangeValue & lv,int& property)
      {
        int odds = lv.per;
        //    int odds = odds_of_property(lv.per,property);
        //    Zebra::logger->debug("属性产生概率%f,%f",lv.per*1.0,odds*1.0);
        if (selectByTenTh(odds) )  {
          ++property;

          ret += randBetween(lv.data.min,lv.data.max);

          /*
          if (selectByPercent(_odds.sleight) ) {
            ret += lv.sleightValue;
            return true;
          }
          // */
        }

        return false;
      }  

    template <typename T>
      bool additive(T& ret,const luckRangeValue & lv)
      {
        if (selectByTenTh(lv.per) )  {
          ret += randBetween(lv.data.min,lv.data.max);

        }
        /*
        if (selectByPercent(_odds.sleight) ) {
          ret += lv.sleightValue;
          return true;
        }
        // */

        return false;
      }  
    template <typename T>
      bool additivePercent(T& ret,const luckRangeValue & lv)
      {
        ret += randBetween(lv.data.min,lv.data.max);
        /*
        if (selectByPercent(_odds.sleight) ) {
          ret += lv.sleightValue;
          return true;
        }
        // */

        return false;
      }  

};

/**
 * \brief 物品改造
 *
 * 实现各种物品改造功能,提供一个统一入口
 *
 */    
class RebuildObject : public Base
{
public:    
  enum {
    MAX_NUMBER = 50,
    
    HOLE_MONEY = 1000,
    ENCHANCE_MONEY = 500,
    
    COMP_SOUL_STONE_ID = 677,
    ENCHANCE_SONTE_ID = 678,
    HOLE_SONTE_ID = 679,
    LEVELUP_STONE_ID = 681,
  };
  
  static RebuildObject& instance();
      
  bool compose(SceneUser& user,const Cmd::stPropertyUserCmd* cmd);
  
  bool compose_soul_stone(SceneUser& user,const Cmd::stPropertyUserCmd* cmd);
  
  bool upgrade(SceneUser& user,const Cmd::stPropertyUserCmd* cmd);
  
  //sky 老的打造不支持批量打造
  //bool make(SceneUser& user,const Cmd::stReMakUserCmd* cmd);

  //sky 新的打造函数
  int make(SceneUser& user, DWORD dwID, int num, BYTE kind);
  
  bool hole(SceneUser& user,const Cmd::stPropertyUserCmd* cmd);
  
  bool enchance(SceneUser& user,const Cmd::stPropertyUserCmd* cmd);

  bool decompose(SceneUser& user,const Cmd::stPropertyUserCmd* cmd);

  //zObject *RebuildObject::remake(SceneUser& user,const Cmd::stReMakObjectUserCmd* cmd,DWORD id);
private:  
  /**     
   * \brief 构造函数
   *
   */     
  RebuildObject() { }
  
  /**     
   * \brief 析构函数
   *
   */     
  ~RebuildObject() { }

  
  static RebuildObject* _instance;
};
namespace Op {
  /**
   * \brief 变量大小判定
   *
   * 判断一个变量是否大于给定值
   *
   */
  template <typename T>
  class Great
  {
  public:
    enum {
      NEED_EXIST = 1,
    };
    
    typedef T value_type;
    
    /**     
     * \brief 判断给定变量是否大于测试值
     *
     * \param value: 待判断变量
     * \param condition: 测试值
     * \return 待判断变量大于测试值返回true,否则返回false
     */         
    bool operator() (T value,T condition) const
    {
      return (value>condition);
    }
    
    //added for debug
    std::string name() const 
    {
      return "Great";
    }
  };
  
  /**
   * \brief 变量大小判定
   *
   * 判断一个变量是否小于给定值
   *
   */
  template <typename T>
  class Less
  {
  public:
    enum {
      NEED_EXIST = 0,
    };

    typedef T value_type;
    
    /**     
     * \brief 判断给定变量是否小于测试值
     *
     * \param value: 待判断变量
     * \param condition: 测试值
     * \return 待判断变量小于测试值返回true,否则返回false
     */         
    bool operator() (T value,T condition) const
    {
      return (value<condition);
    }

    //added for debug
    std::string name() const 
    {
      return "Less";
    }
  };
  
  /**
   * \brief 变量大小判定
   *
   * 判断一个变量是否等于给定值
   *
   */
  template <typename T>
  class Equal
  {
  public:
    enum {
      NEED_EXIST = 1,
    };

    typedef T value_type;
    
    /**     
     * \brief 判断给定变量是否等于测试值
     *
     * \param value: 待判断变量
     * \param condition: 测试值
     * \return 待判断变量等于测试值返回true,否则返回false
     */         
    bool operator() (T value,T condition) const
    {
      return (value==condition);
    }

    //added for debug
    std::string name() const 
    {
      return "Equal";
    }
    
  };
  
  /**
   * \brief 变量大小判定
   *
   * 判断一个变量不同于给定值
   *
   */
  template <typename T>
  class Differ
  {
  public:
    enum {
      NEED_EXIST = 0,
    };

    typedef T value_type;
    
    /**     
     * \brief 判断给定变量是否不等于测试值
     *
     * \param value: 待判断变量
     * \param condition: 测试值
     * \return 待判断变量不等于测试值返回true,否则返回false
     */         
    bool operator() (T value,T condition) const
    {
      return (value!=condition);
    }

    //added for debug
    std::string name() const 
    {
      return "Differ";
    }
    
  };
}

/**
 * \brief 触发条件基类
 *
 * 该类定义了脚本触发条件的接口。
 *
 */  
class Condition
{
public:
  typedef Condition Base;

  virtual bool is_valid (const SceneUser* user,const Vars* vars) const;

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~Condition() { }
  
protected:
  virtual bool check_args(const SceneUser* user,const Vars* vars) const;
  
  virtual bool check_valid(const SceneUser* user,const Vars* vars) const = 0;
};

/**
 * \brief 关键词列表类
 *
 * 存储了关键词的列表,并提供了对应的操作.
 *
 */
class ScenesParser
{
public:

  /**     
   * \brief 取得xml节点内容
   *
   * 遍历关键词列表,取得他们对应的值
   *      
   * \param xml: 目标xml文件
   * \param node: 目标节点
   * \return 当前总是返回true
   */       
  bool parse (zXMLParser& xml,xmlNodePtr& node)
  {    
    for(iterator it=_kvs.begin(); it!=_kvs.end(); ++it) {
      if (!xml.getNodePropStr(node,it->first.c_str(),it->second)) {  
        _kvs[it->first] = "0";
        //return false;
      }
    }

    return true;
  }
  

  /**     
   * \brief 设置一个关键词
   *
   * 在关键词列表中增加一个关键词,对应的值为"0"
   *      
   * \param key: 关键词名称
   * \return 无
   */       
  void key(const std::string& key) 
  {
    _kvs[key] = "0";
  }

  /**     
   * \brief 取值
   *
   * 在关键词列表中搜寻对应的关键词,并返回对应的值,没找到返回零值
   *      
   * \param key: 关键词名称
   * \param value: 取得的值
   * \return 无
   */  
  template <typename T>
  void value(const std::string& key,T& value)
  {
    std::stringstream os(_kvs[key]);  
    os >> value;
  }
  
  
private:
  typedef std::map<std::string,std::string> KV;
  typedef KV::iterator iterator;
  KV _kvs;  
};

/**
 * \brief 组队触发条件基类
 *
 * 该类提供了对于组队任务的支持
 *
 */  
class TeamCondition : public Condition
{
public:
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  TeamCondition(ScenesParser& p)
  { 
    p.value("team",_team);
  }
  
  /**     
   * \brief 析构函数
   *
   */    
  virtual ~TeamCondition() { }
  
  virtual bool is_valid(const SceneUser* user,const Vars* vars) const;
  
protected:  
  
private:
  int _team;
};

/**
 * \brief 变量条件
 *
 * 该类提供了对于任务脚本中有关变量的条件判定的支持
 *
 */
template <typename Operation>
class VarCondition : public TeamCondition
{
public:
  typedef typename Operation::value_type value_type;

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  VarCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("name",_name);
    p.value("value",_condition);
    p.value("id",_id);
  }
  
  /**     
   * \brief 析构函数
   *
   */    
  virtual ~VarCondition()
  { }

  
  /**     
   * \brief  变量判定
   *
   * 重载了check_valid函数,判定某个变量是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid (const SceneUser* user,const Vars* vars) const
  {
    Operation op;
//    Zebra::logger->debug("id:%d\tname:%s\tcondition:%d\toperation:%s",_id,_name.c_str(),_condition,op.name().c_str());
    if (_id) {
      Vars* v = user->quest_list.vars(_id);
      if (v) {
        return v->is_valid(op,_name,_condition);
      }else {
        return !Operation::NEED_EXIST;
      }
    }
      
    return vars->is_valid(op,_name,_condition);
  };

private:
  std::string _name;
  value_type _condition;
  int _id;
};

/**
 * \brief 用户变量条件
 *
 * 该类提供了对于任务脚本中有关用户变量的条件判定的支持
 *
 */
template <typename Operation>
class UserVarCondition : public TeamCondition
{
public:
  typedef typename Operation::value_type value_type;

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  UserVarCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("name",_name);
    p.value("value",_condition);
    p.value("id",_id);
  }
  
  /**     
   * \brief 析构函数
   *
   */    
  virtual ~UserVarCondition()
  { }

  /**     
   * \brief  用户变量判定
   *
   * 重载了check_valid函数,判定某个用户变量是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid (const SceneUser* user,const Vars* vars) const
  {
    Operation op;
    int id = _id?_id:vars->quest_id();

    QWORD key = ((QWORD)user->charbase.accid << 32) | user->charbase.id;
    Vars* v = UserVar::instance().vars(id,key);
    if (v) {
      return v->is_valid(op,_name,_condition);
    }

    return !Operation::NEED_EXIST;
    
  };

private:
  std::string _name;
  value_type _condition;
  int _id;
};

/**
 * \brief 全局变量条件
 *
 * 该类提供了对于任务脚本中有关全局变量的条件判定的支持
 *
 */
template <typename Operation>
class GlobalCondition : public Condition
{
public:
  typedef typename Operation::value_type value_type;

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  GlobalCondition(ScenesParser& p)
  { 
    p.value("name",_name);
    p.value("value",_condition);
    p.value("id",_id);
  }
  
  /**     
   * \brief 析构函数
   *
   */    
  virtual ~GlobalCondition()
  { }

  /**     
   * \brief  参数检查
   *
   * 重载了check_args函数,该条件不需要用户信息
   *      
   * \param user: NULL
   * \param vars: 变量信息
   * \return 变量有效返回true,否则返回false
   */   
  bool check_args(const SceneUser* user,const Vars* vars) const
  {
    if (!vars) return false;
    
    return true;
  }

  /**     
   * \brief  全局变量判定
   *
   * 重载了check_valid函数,判定某个全局变量是否满足脚本要求
   *      
   * \param user: NULL
   * \param vars: 变量信息
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid (const SceneUser* user,const Vars* vars) const
  {
    Operation op;
    int id = _id?_id:vars->quest_id();

//    Zebra::logger->debug("id:%d\tname:%s\tcondition:%d\toperation:%s",id,_name.c_str(),_condition,op.name().c_str());
    
    Vars* v = GlobalVar::instance().vars(id);
    if (v) {
      return v->is_valid(op,_name,_condition);
    }
    
    return !Operation::NEED_EXIST;
  };

protected:
  std::string _name;
  value_type _condition;
  int _id;
};

template <typename Operation>
class TongVarCondition : public GlobalCondition<Operation>
{
public:
  TongVarCondition(ScenesParser& p) : GlobalCondition<Operation>(p)
  { 
  }

  virtual ~TongVarCondition()
  { }

  bool check_args(const SceneUser* user,const Vars* vars) const
  {
    if (!user) return false;
    
    return true;
  }

  bool check_valid (const SceneUser* user,const Vars* vars) const
  {
    Operation op;

    Vars* v = TongVar::instance().vars(user->charbase.unionid);
    if (v) {
      return v->is_valid(op,GlobalCondition<Operation>::_name,GlobalCondition<Operation>::_condition);
    }
    
    return !Operation::NEED_EXIST;
  };
};

template <typename Operation>
class FamilyVarCondition : public TongVarCondition<Operation>
{
public:
  FamilyVarCondition(ScenesParser& p) : TongVarCondition<Operation>(p)
  { 
  }

  virtual ~FamilyVarCondition()
  { }

  bool check_valid (const SceneUser* user,const Vars* vars) const
  {
    Operation op;

    Vars* v = FamilyVar::instance().vars(user->charbase.septid);
    if (v) {
      return v->is_valid(op,GlobalCondition<Operation>::_name,GlobalCondition<Operation>::_condition);
    }
    
    return !Operation::NEED_EXIST;
  };
};

template <typename Operation>
class UsersVarCondition : public TongVarCondition<Operation>
{
public:
  UsersVarCondition(ScenesParser& p) : TongVarCondition<Operation>(p)
  { 
  }

  virtual ~UsersVarCondition()
  { }

  bool check_valid (const SceneUser* user,const Vars* vars) const
  {
    Operation op;

    Vars* v = UsersVar::instance().vars( ((QWORD)user->charbase.accid << 32) | user->charbase.id);
    if (v) {
      return v->is_valid(op,GlobalCondition<Operation>::_name,GlobalCondition<Operation>::_condition);
    }
    
    return !Operation::NEED_EXIST;
  };
};

/**
 * \brief 技能是否学习条件
 *
 * 该类提供了判断用户是否学习技能
 *
 */
template <typename Operation>
class HaveSkillCondition : public TeamCondition
{
public:  
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  HaveSkillCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_condition);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~HaveSkillCondition()
  { }
  
  /**     
   * \brief  技能个数判定
   *
   * 重载了check_valid函数,判定用户的技能数量是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    Operation op;
    int size=user->usm.size();
    return op(size,_condition);
  }

private:
  value_type _condition;    
};


/**
 * \brief 检查玩家性别
 *
 * 该类提供了判断用户的性别
 *
 */
template <typename Operation>
class CheckSexCondition : public TeamCondition
{
public:  
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  CheckSexCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_condition);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~CheckSexCondition()
  { }
  
  /**     
   * \brief  技能个数判定
   *
   * 重载了check_valid函数,判定用户的技能数量是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    Operation op;
    int sex = 0;
    switch(user->charbase.type)
    {
      case PROFESSION_1:    //侠客
      case PROFESSION_3:    //箭侠
      case PROFESSION_5:    //天师
      case PROFESSION_7:    //法师
        sex = 1;
        break;
      case PROFESSION_2:    //侠女
      case PROFESSION_4:    //箭灵
      case PROFESSION_6:    //美女
      case PROFESSION_8:    //仙女
        sex = 0;
        break;
      case PROFESSION_NONE:  //无业
      default:
        break;
    }
    return op(sex,_condition);
  }

private:
  value_type _condition;    
};

/**
 * \brief 等级条件
 *
 * 该类提供了判断用户等级是否大于,小于,等于或不等于某个值的接口
 *
 */
template <typename Operation>
class LevelCondition : public TeamCondition
{
public:  
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  LevelCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_condition);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~LevelCondition()
  { }
  
  /**     
   * \brief  等级判定
   *
   * 重载了check_valid函数,判定用户的等级是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    Operation op;
    return op(user->charbase.level,_condition);
  }

private:
  value_type _condition;    
};


/**
 * \brief 家族条件
 *
 * 该类提供了判断用户家族ID是否大于,小于,等于或不等于某个值的接口
 *
 */
template <typename Operation>
class SeptIDCondition : public TeamCondition
{
public:  
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  SeptIDCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_condition);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~SeptIDCondition()
  { }
  
  /**     
   * \brief  家族判定
   *
   * 重载了check_valid函数,判定用户的家族ID是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    Operation op;
    return op(user->charbase.septid,_condition);
  }

private:
  value_type _condition;    
};

/**
 * \brief 帮会条件
 *
 * 该类提供了判断用户家族ID是否大于,小于,等于或不等于某个值的接口
 *
 */
template <typename Operation>
class UnionIDCondition : public TeamCondition
{
public:  
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  UnionIDCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_condition);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~UnionIDCondition()
  { }
  
  /**     
   * \brief  帮会判定
   *
   * 重载了check_valid函数,判定用户的家族ID是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    Operation op;
    return op(user->charbase.unionid,_condition);
  }

private:
  value_type _condition;    
};

/**
 * \brief 金钱条件
 *
 * 该类提供了判断用户包裹中金钱数量是否大于,小于,等于或不等于某个值的接口
 *
 */
template <typename Operation>
class GoldCondition : public TeamCondition
{
public:  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  GoldCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_condition);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~GoldCondition()
  { }

  /**     
   * \brief  金钱判定
   *
   * 重载了check_valid函数,判定用户包裹中的金钱是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    zObject* gold = const_cast<SceneUser *>(user)->packs.getGold();
    
    DWORD number = 0;
    if (gold)  number = gold->data.dwNum;
    Operation op;

    //shouldn't be exist,NB CEHUA
    //if (op.name() == "Great" && number < (DWORD)_condition) {
      //Channel::sendSys(const_cast<SceneUser*>(user),Cmd::INFO_TYPE_FAIL,"金钱不足");
    //}
    
    return op(number,_condition);
  }

private:
  int _condition;    
};

/**
 * \brief 物品条件
 *
 * 该类提供了判断用户包裹中的特定物品数量是否大于,小于,等于或不等于某个值的接口
 *
 */
class ItemCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  ItemCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("id",_id);
    p.value("value",_condition);
    p.value("level",_level);
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~ItemCondition()
  { }

  /**     
   * \brief  物品数量判定
   *
   * 重载了check_valid函数,判定用户包裹空间的某个物品数量是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    return user->packs.uom.exist(_id,_condition,_level);
  }

private:
  int _id;
  int _level;
  value_type _condition;    
};

/**
 * \brief 国家条件
 *
 * 该类提供了判断用户是否属于一个国家的接口
 *
 */
class NationCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  NationCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_condition);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~NationCondition()
  { }
  
  /**     
   * \brief  国家判定
   *
   * 重载了check_valid函数,判定用户是否属于某个国家
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    return (int)user->charbase.country == _condition;
  }

private:
  value_type _condition;    
};

/**
 * \brief 是否在本国条件
 *
 * 该类提供了判断用户是否在本国的接口
 *
 */
class InNationCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  InNationCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_condition);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~InNationCondition()
  { }
  
  /**     
   * \brief  国家判定
   *
   * 重载了check_valid函数,判定用户是否属于某个国家
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    return user->charbase.country == user->scene->getCountryID();
  }

private:
  value_type _condition;    
};

/**
 * \brief 帮会条件
 *
 * 该类提供了判断用户是否属于某个帮会的接口
 *
 */
class ConfraternityCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  ConfraternityCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_condition);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~ConfraternityCondition()
  { }
  
  /**     
   * \brief  帮会判定
   *
   * 重载了check_valid函数,判定用户是否属于某个帮会
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    return (int)(user->charbase.unionid & 0x1) == _condition;
  }

private:
  value_type _condition;    
};

/**
 * \brief 职业条件
 *
 * 该类提供了判断用户是否属于某个职业的接口
 *
 */
class ProfessionCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  ProfessionCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_condition);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~ProfessionCondition()
  { }
  
  /**     
   * \brief  职业判定
   *
   * 重载了check_valid函数,判定用户职业是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    return user->charbase.type & _condition;
  }

private:
  value_type _condition;    
};

/**
 * \brief 包裹空间条件
 *
 * 该类提供了判断用户包裹中的空间是否能容纳某个物品的接口
 *
 */
class SpaceCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  SpaceCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("size",_size);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~SpaceCondition()
  { }
  
  /**     
   * \brief  包裹空间判定
   *
   * 重载了check_valid函数,判定用户包裹空间是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    int free = user->packs.uom.space(user);

    if (free >= _size)   return true;

    //Channel::sendSys(const_cast<SceneUser*>(user),Cmd::INFO_TYPE_FAIL,"包裹空间不足");
    return false;
  }

private:
  value_type _size;    
};

/**
 * \brief 是否超过任务时间要求
 *
 * 该类提供了判断任务时间是否满足要求的接口
 *
 */
class TimeoutsCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  TimeoutsCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("long",_time);  
    p.value("id",_id);  
    p.value("less",_less);
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~TimeoutsCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定任务时间是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    if (_id) {
      Vars* v = user->quest_list.vars(_id);
      if (v) {
        return _less?(v->is_timeout(_time)):(!v->is_timeout(_time));
      }
      return true;
    }
      
    return _less?(vars->is_timeout(_time)):(!vars->is_timeout(_time));
  }

private:
  value_type _time;    
  int _id;
  int _less;
};

/**
 * \brief 时间条件
 *
 * 该类提供了判断系统时间是否满足要求的接口
 *
 */
class TimerCondition : public Condition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  TimerCondition(ScenesParser& p)
  { 
    std::string start;
    p.value("start",start);
    strptime(start.c_str(),"%H:%M:%S",&_start);    
    std::string end;
    p.value("end",end);
    strptime(end.c_str(),"%H:%M:%S",&_end);        
  }


  /**     
   * \brief 析构函数
   *
   */    
  virtual ~TimerCondition()
  { }

  /**     
   * \brief  参数检查
   *
   * 重载了check_args函数,该条件不需要用户及变量信息
   *      
   * \param user: NULL
   * \param vars: NULL
   * \return 当前总是返回true
   */   
  bool check_args(const SceneUser* user,const Vars* vars) const
  {
    return true;
  }

  /**     
   * \brief  时间条件判定
   *
   * 重载了check_valid函数,判定系统时间是否满足脚本要求
   *      
   * \param user: NULL
   * \param vars: NULL
   * \return true表示满足条件,false表示不满足条件
   */     
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
          struct tm now;
          time_t timValue = time(NULL);
          zRTime::getLocalTime(now,timValue);
    //Zebra::logger->debug("Time_START:%02d:%02d:%02d",_start.tm_hour,_start.tm_min,_start.tm_sec);
    //Zebra::logger->debug("Time_NOW  :%02d:%02d:%02d",now.tm_hour,now.tm_min,now.tm_sec);
    //Zebra::logger->debug("Time_END  :%02d:%02d:%02d",_end.tm_hour,_end.tm_min,_end.tm_sec);
    if (((now.tm_hour > _start.tm_hour) || ( (now.tm_hour == _start.tm_hour) && (now.tm_min > _start.tm_min) ) 
      || ((now.tm_hour == _start.tm_hour) && (now.tm_min == _start.tm_min) && (now.tm_sec >= _start.tm_sec))) //start
      && ((now.tm_hour < _end.tm_hour) || ( (now.tm_hour == _end.tm_hour) && (now.tm_min < _end.tm_min) ) 
      || ((now.tm_hour == _end.tm_hour) && (now.tm_min == _end.tm_min) && (now.tm_sec < _end.tm_sec)))    //end
      ) {
      return true;
    }  

    return false;  
  }

private:
  struct tm _start;
  struct tm _end;  
};

/**
 * \brief 组队条件
 *
 * 该类提供了判断用户队伍是否满足要求的接口
 *
 */
class TeamedCondition : public Condition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  TeamedCondition(ScenesParser& p) 
  { 
    p.value("number",_number);  
    p.value("male",_male);
    p.value("female",_female);
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~TeamedCondition()
  { }
  
  bool check_valid(const SceneUser* user,const Vars* vars) const;

private:
  value_type _number;  
  value_type _male;
  value_type _female;
};

class IsGuardCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  IsGuardCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_need);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~IsGuardCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定任务时间是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    int guard = user->guard?1:0;
    return guard == _need;
  }
private:
  value_type _need;
};

class FiveTypeCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  FiveTypeCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_need);  
  }
  
  /**     
   * \brief 析构函数
   *
   */    
  virtual ~FiveTypeCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定任务时间是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    return (int)user->charbase.fivetype == _need;
  }
private:
  value_type _need;
};

template <typename Operation>
class FiveLevelCondition : public TeamCondition
{
public:
  typedef typename Operation::value_type value_type;

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  FiveLevelCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_level);
  }
  
  /**     
   * \brief 析构函数
   *
   */    
  virtual ~FiveLevelCondition()
  { }

  
  /**     
   * \brief  变量判定
   *
   * 重载了check_valid函数,判定某个变量是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid (const SceneUser* user,const Vars* vars) const
  {
    Operation op;
      
    return op((int)user->charbase.fivelevel,_level);
  };

private:
  value_type _level;
};

template <typename Operation>
class FamilyLevelCondition : public TeamCondition
{
public:
  typedef typename Operation::value_type value_type;

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  FamilyLevelCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_level);
  }
  
  /**     
   * \brief 析构函数
   *
   */    
  virtual ~FamilyLevelCondition() { }

  
  /**     
   * \brief  变量判定
   *
   * 重载了check_valid函数,判定某个变量是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid (const SceneUser* user,const Vars* vars) const
  {
    Operation op;
      
    return op((int)user->dwSeptLevel,_level);
  };

private:
  value_type _level;
};

template <typename Operation>
class ReputeCondition : public TeamCondition
{
public:
  typedef typename Operation::value_type value_type;

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  ReputeCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_level);
  }
  
  /**     
   * \brief 析构函数
   *
   */    
  virtual ~ReputeCondition()
  { }

  
  /**     
   * \brief  变量判定
   *
   * 重载了check_valid函数,判定某个变量是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid (const SceneUser* user,const Vars* vars) const
  {
    Operation op;
      
    return op((int)user->dwSeptRepute,_level);
  };

private:
  value_type _level;
};

template <typename Operation>
class ActionPointCondition : public TeamCondition
{
public:
  typedef typename Operation::value_type value_type;

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  ActionPointCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_level);
  }
  
  /**     
   * \brief 析构函数
   *
   */    
  virtual ~ActionPointCondition()
  { }

  
  /**     
   * \brief  变量判定
   *
   * 重载了check_valid函数,判定某个变量是否满足脚本要求
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid (const SceneUser* user,const Vars* vars) const
  {
    Operation op;
      
    return op((int)user->dwUnionActionPoint,_level);
  };

private:
  value_type _level;
};

class HorseCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  HorseCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_id);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~HorseCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定用户是否拥有所需马匹
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    return user->horse.horse() == (DWORD)_id;
  }
private:
  value_type _id;
};

class GradeCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  GradeCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_grade);  
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~GradeCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定用户是否拥有所需马匹
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const
  {
    if (_grade == 1) return user->king;
    if (_grade == 2) return user->unionMaster;
    if (_grade == 4) return user->septMaster;
    
    return true;
  }
private:
  value_type _grade;
};

class MapCountryCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  MapCountryCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("name",_name);
    p.value("id",_id);
  }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~MapCountryCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定用户是否拥有所需马匹
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const;
private:
  std::string _name;
  int _id;
};

class HonorCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  HonorCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_value);
   }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~HonorCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定用户是否拥有所需马匹
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const;
private:
  int _value;
};

class MaxHonorCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  MaxHonorCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_value);
   }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~MaxHonorCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定用户是否拥有所需马匹
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const;
private:
  int _value;
};

class SelfCountryCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  SelfCountryCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_value);
   }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~SelfCountryCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定用户是否拥有所需马匹
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const;
private:
  int _value;
};

class CountryPowerCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  CountryPowerCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_value);
   }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~CountryPowerCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定国家强弱
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const;
private:
  int _value;
};


class WeekCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  WeekCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_value);
   }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~WeekCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定今天是否是一星期中指定的那几天 value值中用0-6位表示一周的7天
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const;
private:
  int _value;
};


class CaptionCondition : public TeamCondition
{
public:  
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  CaptionCondition(ScenesParser& p) : TeamCondition(p)
  { 
    p.value("value",_value);
   }

  /**     
   * \brief 析构函数
   *
   */    
  virtual ~CaptionCondition()
  { }
  
  /**     
   * \brief  任务时间判定
   *
   * 重载了check_valid函数,判定角色是否国王或者城主
   *      
   * \param user: 触发条件的用户
   * \param vars: 用户所带的该任务相关变量
   * \return true表示满足条件,false表示不满足条件
   */    
  bool check_valid(const SceneUser* user,const Vars* vars) const;
private:
  int _value;
};

namespace Op {
  /**
   * \brief 增加变量值
   *
   * 增加一个变量的值
   *
   */
  template <typename T>
  class Add
  {
  public:
    typedef T value_type;

    /**     
     * \brief 
     *
     * 增加变量的值         
     *      
     * \param value: 待改变的变量
     * \param action: 要修改的值
     * \return 无
     */         
    void operator() (T& value,T action,SceneUser *user) const
    {
      value += action;
    }

    //added for debug
    std::string name() const 
    {
      return "Add";
    }    
  };

  /**
   * \brief 设定变量值
   *
   * 设定一个变量的值
   *
   */
  template <typename T>
  class Set
  {
  public:
    typedef T value_type;
    
    /**     
     * \brief  
     *
     * 设定一个变量的值         
     *      
     * \param value: 待改变的变量
     * \param action: 要设定的值
     * \return 无
     */     
     void operator() (T& value,T action,SceneUser *user) const
    {
      value = action;
    }

    //added for debug
    std::string name() const 
    {
      return "Set";
    }    
    
  };

  /**
   * \brief 减少变量值
   *
   * 减少一个变量的值
   *
   */  
  template <typename T>
  class Sub
  {
  public:
    typedef T value_type;
    
    /**     
     * \brief 
     *
     * 增加变量的值         
     *      
     * \param value: 待改变的变量
     * \param action: 要减少的值
     * \return 无
     */     
    void operator() (T& value,T action,SceneUser *user) const
    {
      value -= action;
    }

    //added for debug
    std::string name() const 
    {
      return "Sub";
    }    
  };

  /**
   * \brief 乘一个变量值
   *
   * 乘一个变量的值
   *
   */
  template <typename T>
  class Mul
  {
  public:
    typedef T value_type;
    
    /**     
     * \brief  
     *
     * 设定一个变量的值         
     *      
     * \param value: 待改变的变量
     * \param action: 要设定的值
     * \return 无
     */     
     void operator() (T& value,T action,SceneUser *user) const
    {
      value *= action;
    }

    //added for debug
    std::string name() const 
    {
      return "Mul";
    }    
  };

  /**
   * \brief 变量值乘方
   *
   * 变量值乘方
   *
   */
  template <typename T>
  class Pow
  {
  public:
    typedef T value_type;
    
    /**     
     * \brief  
     *
     * 设定一个变量的值         
     *      
     * \param value: 待改变的变量
     * \param action: 要设定的值
     * \return 无
     */     
     void operator() (T& value,T action,SceneUser *user) const
    {
      value *= value;
    }

    //added for debug
    std::string name() const 
    {
      return "Pow";
    }    
  };

  /**
   * \brief 变量值乘方
   *
   * 变量值乘方
   *
   */
  template <typename T>
  class Div
  {
  public:
    typedef T value_type;
    
    /**     
     * \brief  
     *
     * 设定一个变量的值         
     *      
     * \param value: 待改变的变量
     * \param action: 要设定的值
     * \return 无
     */     
     void operator() (T& value,T action,SceneUser *user) const
    {
      value /= action;
    }

    //added for debug
    std::string name() const 
    {
      return "Div";
    }    
  };

  /**
   * \brief 变量值乘方
   *
   * 变量值乘方
   *
   */
  template <typename T>
  class GetP
  {
  public:
    typedef T value_type;
    
    /**     
     * \brief  
     *
     * 设定一个变量的值         
     *      
     * \param value: 待改变的变量
     * \param action: 要设定的值
     * \return 无
     */     
     void operator() (T& value,T action,SceneUser *user) const
    {
      if (user)
      {
        switch(action)
        {
          case 1:
            value = user->charbase.level;
            break;
          case 2:
            value = time(NULL)%2000000000;
            break;
          case 3:
            break;
          default:
            break;
        }
      }
    }

    //added for debug
    std::string name() const 
    {
      return "GetP";
    }    
  };
}

class SceneUser;

/**
 * \brief 触发动作基类
 *
 * 该类定义了脚本触发动作的接口。
 *
 */  
class Action
{
public:  
  typedef Action Base;
  
  enum {
    SUCCESS = 0,
    FAILED = 1,
    DISABLE = 2,    
  };

  virtual int do_it (SceneUser* user,Vars* vars);
  

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~Action() { }

protected:
  virtual bool check_args(SceneUser* user,Vars* vars) const;
  
  /**     
   * \brief  执行脚本定义的动作
   *
   * 虚函数,继承类需要重载此函数提供对各种触发动作的支持
   *      
   * \param user: 触发动作的用户
   * \param vars: 用户所带的该任务相关变量
   * \return SUCCESS表示成功,FAILED表示失败,DISABLE表示禁用某项功能
   */     
  virtual int done(SceneUser* user,Vars* vars) = 0;
};

/**
 * \brief 组队触发动作基类
 *
 * 该类提供了对于组队任务的支持
 *
 */  
class TeamAction : public Action
{
public:  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  TeamAction(ScenesParser& p)
  {
    p.value("team",_team);  
  }
    
  int do_it (SceneUser* user,Vars* vars);

protected:  
  /**     
   * \brief  执行脚本定义的动作
   *
   * 虚函数,继承类需要重载此函数提供对各种触发动作的支持
   *      
   * \param user: 触发动作的用户
   * \param vars: 用户所带的该任务相关变量
   * \return SUCCESS表示成功,FAILED表示失败,DISABLE表示禁用某项功能
   */     
  int done(SceneUser* user,Vars* vars) = 0;

private:
  
  int _team;
};

/**
 * \brief 变量
 *
 * 该类提供了对于任务脚本中有关变量的修改的支持
 *
 */  
template <typename Operation>
class VarAction : public TeamAction
{
public:
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  VarAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("name",_name);
    p.value("value",_action);
    p.value("id",_id);
    p.value("tmp",_tmp);    
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~VarAction()
  { }

  /**     
   * \brief  执行变量修改
   *
   * 重载了done函数,实现对相关变量的修改
   *      
   * \param user: 触发动作的用户
   * \param vars: 用户所带的该任务相关变量
   * \return SUCCESS表示成功,FAILED表示失败,DISABLE表示禁用某项功能
   */       
  int done (SceneUser* user,Vars* vars)
  {
    Operation op;
//    Zebra::logger->debug("id:%d\tname:%s\taction:%d\toperation:%s",_id,_name.c_str(),_action,op.name().c_str());          
    if (_id) {
      Vars* v = user->quest_list.vars(_id);
      if (v) {
        v->set_value(op,_name,_action,_tmp,user);
      }
    }else {
      vars->set_value(op,_name,_action,_tmp,user);
    }
    return Action::SUCCESS;
  }
  
private:
  std::string _name;
  value_type _action;
  int _tmp;
  int _id;
};

/**
 * \brief 变量间动作
 *
 * 该类提供了对于任务脚本中变量之间的操作的支持
 *
 */  
template <typename Operation>
class VarsAction : public TeamAction
{
public:
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  VarsAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("name1",_name1);
    p.value("id1",_id1);
    p.value("name2",_name2);
    p.value("id2",_id2);
    p.value("tmp",_tmp);    
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~VarsAction()
  { }

  /**     
   * \brief  执行变量修改
   *
   * 重载了done函数,实现对相关变量的修改
   *      
   * \param user: 触发动作的用户
   * \param vars: 用户所带的该任务相关变量
   * \return SUCCESS表示成功,FAILED表示失败,DISABLE表示禁用某项功能
   */       
  int done (SceneUser* user,Vars* vars)
  {
    Operation op;
    value_type _action;
//    Zebra::logger->debug("id:%d\tname:%s\taction:%d\toperation:%s",_id,_name.c_str(),_action,op.name().c_str());          
    if (_id1)
    {
      Vars* v = user->quest_list.vars(_id1);
      if (v)
      {
        if (_id2)
        {
          Vars* tmpvars= user->quest_list.vars(_id2);
          tmpvars->get_value(_name2,_action);
        }
        else
        {
          vars->get_value(_name2,_action);
        }
        v->set_value(op,_name1,_action,_tmp,user);

      }
    }
    else
    {
      if (_id2)
      {
        Vars* tmpvars= user->quest_list.vars(_id2);
        tmpvars->get_value(_name2,_action);
      }
      else
      {
        vars->get_value(_name2,_action);
      }
      vars->set_value(op,_name1,_action,_tmp,user);
    }
    return Action::SUCCESS;
  }
  
private:
  int _tmp;
  int _id1;
  std::string _name1;
  int _id2;
  std::string _name2;
};

/**
 * \brief 用户变量
 *
 * 该类提供了对于任务脚本中需要保持在用户身上变量的支持
 *
 */  
template <typename Operation>
class UserVarAction : public TeamAction
{
public:
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  UserVarAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("name",_name);
    p.value("value",_action);
    p.value("id",_id);
    p.value("tmp",_tmp);    
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~UserVarAction()
  { }

  /**     
   * \brief  执行变量修改
   *
   * 重载了done函数,实现对相关变量的修改
   *      
   * \param user: 触发动作的用户
   * \param vars: 用户所带的该任务相关变量
   * \return SUCCESS表示成功,FAILED表示失败,DISABLE表示禁用某项功能
   */       
  int done (SceneUser* user,Vars* vars)
  {
    Operation op;
    
    if (!_id) _id = vars->quest_id();
        
    Vars* v = UserVar::instance().add(_id,((QWORD)user->charbase.accid << 32) | user->charbase.id);
    v->set_value(op,_name,_action,_tmp,user);
    
    return Action::SUCCESS;
  }
  
private:
  std::string _name;
  value_type _action;
  int _tmp;
  int _id;
};

/**
 * \brief 系统信息
 *
 * 该类提供了对用户提供系统聊天信息的支持.
 *
 */  
class NotifyAction : public TeamAction
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  NotifyAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("content",_info);  
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~NotifyAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  std::string _info;  
};

/**
 * \brief 日志信息
 *
 * 该类提供了对日志信息的支持.
 *
 */  
class LogAction : public TeamAction
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  LogAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("content",_info);  
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~LogAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  std::string _info;  
};

class BulletinAction : public TeamAction
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  BulletinAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("content",_info);  
    p.value("kind",_kind);
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~BulletinAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  std::string _info;  
  int _kind; 
};

/**
 * \brief 系统信息
 *
 * 该类提供了对用户提供系统聊天信息的支持.
 *
 */  
class Notify1Action : public TeamAction
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  Notify1Action(ScenesParser& p) : TeamAction(p)
  { 
    p.value("content",_info);  
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~Notify1Action() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  std::string _info;  
};

/**
 * \brief 菜单
 *
 * 该类提供了对客户端动态菜单的支持.
 *
 */  
class MenuAction : public Action
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param menu: 菜单内容
   */       
  MenuAction(const std::string& menu) : _menu(menu)
  { }
  
  virtual ~MenuAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  std::string _menu;  
};

/**
 * \brief 菜单
 *
 * 该类提供了对客户端动态菜单的支持.
 *
 */  
class SubMenuAction : public Action
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param menu: 菜单内容
   */       
  SubMenuAction(const std::string& menu) : _menu(menu)
  { }
  
  virtual ~SubMenuAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  std::string _menu;  
};

/**
 * \brief 菜单
 *
 * 该类提供了对客户端动态菜单的支持.
 *
 */  
class MainMenuAction : public Action
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param menu: 菜单内容
   */       
  MainMenuAction(const std::string& menu) : _menu(menu)
  { }
  
  virtual ~MainMenuAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  std::string _menu;  
};


/**
 * \brief 变量刷新
 *
 * 该类实现了对任务变量的及时刷新
 *
 */  
class RefreshAction : public TeamAction
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  RefreshAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("name",_name);  
    p.value("id",_id);
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~RefreshAction() { }  
  
  int done (SceneUser* user,Vars* vars);
private:
  std::string _name;
  int _id;
};

/**
 * \brief 经验
 *
 * 该类提供了增加特定用户经验的接口
 *
 */
class ExpAction : public TeamAction
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  ExpAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("value",_exp);  
    p.value("name",_name);
    p.value("id",_id);
    
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~ExpAction() { }  
  
  int done (SceneUser* user,Vars* vars);
protected:
  int _exp;
  std::string _name;
  int _id;
};



class Exp1Action : public ExpAction
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  Exp1Action(ScenesParser& p) : ExpAction(p)
  { 
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~Exp1Action() { }  
  
  int done (SceneUser* user,Vars* vars);
};


/**
 * \brief 银子
 *
 * 该类提供了修改用户身上银子数量的接口
 *
 */  
template <typename Operation>
class GoldAction : public TeamAction
{
public:
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  GoldAction(ScenesParser& p)  : TeamAction(p)
  { 
    p.value("value",_value);
    
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~GoldAction() { }  

  /**     
   * \brief  添加银子
   *
   * 重载了done函数,增加用户身上携带的银子
   *      
   * \param user: 触发动作的用户
   * \param vars: 用户所带的该任务相关变量
   * \return SUCCESS表示成功,FAILED表示失败,DISABLE表示禁用某项功能
   */  
  int done (SceneUser* user,Vars* vars)
  {
    Operation op;

    
    if (op.name() == "Add") {
      user->packs.addMoney(_value,"任务添加");
      return Action::SUCCESS;
    }

    if (op.name() == "Sub") {
      if (!user->packs.removeMoney(_value,"任务减少")) {
        Zebra::logger->fatal("任务银子计算错误: 用户(%s),任务(%d)",user->name,vars->quest_id());
      }
      
      return Action::SUCCESS;
    }
    
    return Action::FAILED;
  }
  
private:
  value_type _value;
};

class EnterSeptGuardAction : public Action
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  EnterSeptGuardAction()
  { 
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~EnterSeptGuardAction() { }  
  
  int done (SceneUser* user,Vars* vars);
private:
};

class FinishSeptGuardAction : public Action
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  FinishSeptGuardAction()
  { 
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~FinishSeptGuardAction() { }  
  
  int done (SceneUser* user,Vars* vars);
};

/**
 * \brief 评价
 *
 * 该类提供了增加特定用户评价的接口,尚未实现
 *
 */
template <typename Operation>
class ScoreAction : public TeamAction
{
public:

  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  ScoreAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("value",_value);  
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~ScoreAction() { }  
  
  int done (SceneUser* user,Vars* vars)
  {
    return Action::SUCCESS;
  }
  
private:
  value_type _value;
};

/**
 * \brief 技能
 *
 * 该类提供了增加特定用户技能等级的接口.
 *
 */
class SkillAction : public TeamAction
{
public:
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  SkillAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("id",_id);
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~SkillAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  value_type _id;
};

/**
 * \brief 禁止
 *
 * 该类提供了禁止用户执行某项动作的方法,当前支持的类型有使用物品,拾取物品及上下马.
 *
 */
class DisableAction : public TeamAction
{
public:
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  DisableAction(ScenesParser& p)  : TeamAction(p)
  { 
  
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~DisableAction() { }
  
  int done (SceneUser* user,Vars* vars);
  
private:

};

/**
 * \brief 地图跳转
 *
 * 该类提供了用户在地图上跳转的接口
 *
 */
class GotoAction : public TeamAction
{
public:
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  GotoAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("name",_name);  
    p.value("pos",_pos);
    p.value("pos1",_pos1);
    p.value("pos2",_pos2);
    p.value("pos3",_pos3);
    p.value("pos4",_pos4);
    p.value("cpos",_cpos);
    p.value("rlen",_rlen);
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~GotoAction() { }
  
  int done (SceneUser* user,Vars* vars);
  
private:
  std::string _name;
  std::string _pos;
  std::string _pos1;
  std::string _pos2;
  std::string _pos3;
  std::string _pos4;
  std::string _cpos;
  std::string _rlen;
};

/**
 * \brief 下马
 *
 * 该类提供了使用户下马的接口
 *
 */
class RideDownAction : public TeamAction
{
public:
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  RideDownAction(ScenesParser& p) : TeamAction(p)
  { }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~RideDownAction() { }
  
  int done (SceneUser* user,Vars* vars);
  
private:

};

/**
 * \brief 添加物品
 *
 * 该类提供了给用户添加某个物品的接口
 *
 */
class AddItemAction : public TeamAction
{
public:
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  AddItemAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("id",_id);
    p.value("value",_value);  
    p.value("odds",_odds);
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~AddItemAction() { }
  
  int done (SceneUser* user,Vars* vars);
  
private:
  int _id;
  int _value;
  int _odds;
};

/**
 * \brief 添加绑定物品
 *
 * 该类提供了给用户添加某个物品的接口
 *
 */
class AddBindItemAction : public TeamAction
{
public:
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  AddBindItemAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("id",_id);
    p.value("value",_value);  
    p.value("odds",_odds);
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~AddBindItemAction() { }
  
  int done (SceneUser* user,Vars* vars);
  
private:
  int _id;
  int _value;
  int _odds;
};

/**
 * \brief 添加绿色绑定物品
 *
 * 该类提供了给用户添加某个物品的接口
 *
 */
class AddGreenBindItemAction : public TeamAction
{
public:
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  AddGreenBindItemAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("id",_id);
    p.value("value",_value);  
    p.value("odds",_odds);
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~AddGreenBindItemAction() { }
  
  int done (SceneUser* user,Vars* vars);
  
private:
  int _id;
  int _value;
  int _odds;
};

/**
 * \brief 删除物品
 *
 * 该类提供了删除用户身上某个物品的接口
 *
 */
class RemoveItemAction : public TeamAction
{
public:
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  RemoveItemAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("id",_id);
    p.value("value",_value);      
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~RemoveItemAction() { }
  
  int done (SceneUser* user,Vars* vars);
  
private:
  int _id;
  int _value;
};

/**
 * \brief 丢弃物品
 *
 * 该类提供了丢弃用户身上某个物品的接口
 *
 */
class DropItemAction : public TeamAction
{
public:
  typedef int value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  DropItemAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("id",_id);
    p.value("level",_level);
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~DropItemAction() { }
  
  int done (SceneUser* user,Vars* vars);
  
private:
  int _id;
  int _level;
};

/**
 * \brief 未实现
 *
 * 
 *
 */
class DropAction : public Action
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  DropAction(ScenesParser& p)
  {
    p.value("id",_id);
    p.value("value",_value);  
    p.value("odds",_odds);
    p.value("guard",_guard);
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~DropAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  int _id;
  int _value;
  int _odds;
  int _guard;
};

/**
 * \brief 定时器
 *
 * 该类提供了对需要时间限制的任务的支持
 *
 */
class TimeoutsAction : public Action
{
public:

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  TimeoutsAction(ScenesParser& p)
  {
    //p.value("value",_timeout);
    p.value("id",_id);
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~TimeoutsAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  //int _timeout;
  int _id;
};

/**
 * \brief 设置状态
 *
 * 该类提供了设置用户身上某个状态的接口
 *
 */
class SetStateAction : public TeamAction
{
public:

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  SetStateAction(ScenesParser& p) : TeamAction(p)
  {
    p.value("value",_state);
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~SetStateAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  int _state;
};

/**
 * \brief 清除状态
 *
 * 该类提供了清除用户身上某个状态的接口
 *
 */
class ClearStateAction : public TeamAction
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  ClearStateAction(ScenesParser& p) : TeamAction(p)
  {
    p.value("value",_state);
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~ClearStateAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  int _state;
};

/**
 * \brief 马匹
 *
 * 该类提供了给予用户马匹的接口
 *
 */
class HorseAction : public TeamAction
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  HorseAction(ScenesParser& p) : TeamAction(p)
  {
    p.value("value",_id);
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~HorseAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  int _id;
};


/**
 * \brief 全局变量
 *
 * 该类提供了对于任务脚本中需要对所有用户可见变量的支持
 *
 */  
template <typename Operation>
class GlobalAction : public Action
{
public:
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  GlobalAction(ScenesParser& p) : _id(0)
  { 
    p.value("name",_name);
    p.value("value",_action);
    p.value("id",_id);
    p.value("tmp",_tmp);    
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~GlobalAction()
  { }
  
  bool check_args(SceneUser* user,Vars* vars) const
  {
    if (!vars) return false;
    
    return true;    
  }

  /**     
   * \brief  执行变量修改
   *
   * 重载了done函数,实现对相关变量的修改
   *      
   * \param user: 触发动作的用户
   * \param vars: 用户所带的该任务相关变量
   * \return SUCCESS表示成功,FAILED表示失败,DISABLE表示禁用某项功能
   */       
  int done (SceneUser* user,Vars* vars)
  {
    Operation op;
    if (!_id) _id = vars->quest_id();

//    Zebra::logger->debug("id:%d\tname:%s\taction:%d\toperation:%s",_id,_name.c_str(),_action,op.name().c_str());
        
    Vars* v = GlobalVar::instance().add(_id);
    v->set_value(op,_name,_action,_tmp,user);

    return Action::SUCCESS;
  }
  
protected:
  std::string _name;
  value_type _action;
  int _tmp;
  int _id;
};

template <typename Operation>
class TongVarAction : public GlobalAction<Operation>
{
public:
  typedef typename Operation::value_type value_type;
  
  TongVarAction(ScenesParser& p) : GlobalAction<Operation>(p)
  { 
  }

  virtual ~TongVarAction()
  { }
  
  bool check_args(SceneUser* user,Vars* vars) const
  {
    if (!user) return false;
    
    return true;    
  }

  int done (SceneUser* user,Vars* vars)
  {
    Operation op;
        
    Vars* v = TongVar::instance().add(user->charbase.unionid);
    v->set_value(op,GlobalAction<Operation>::_name,GlobalAction<Operation>::_action,GlobalAction<Operation>::_tmp,user);

    return Action::SUCCESS;
  }  
};

template <typename Operation>
class FamilyLevelAction : public TongVarAction<Operation>
{
public:
  typedef typename Operation::value_type value_type;
  
  FamilyLevelAction(ScenesParser& p) : TongVarAction<Operation>(p),_level(0)
  { 
    p.value("value",_level);
    Zebra::logger->debug("初始化_level%d",_level);
  }

  virtual ~FamilyLevelAction()
  { }

  int done (SceneUser* user,Vars* vars)
  {
    Cmd::Session::t_OpLevel_SceneSession uplevel;
    uplevel.dwSeptID=user->charbase.septid;
    uplevel.dwLevel=_level;
    Zebra::logger->debug("发送_level%d",_level);
    sessionClient->sendCmd(&uplevel,sizeof(uplevel));
    return Action::SUCCESS;
  }
  
private:
  value_type _level;
};

template <typename Operation>
class FamilyVarAction : public TongVarAction<Operation>
{
public:
  typedef typename Operation::value_type value_type;
  
  FamilyVarAction(ScenesParser& p) : TongVarAction<Operation>(p)
  { 
  }

  virtual ~FamilyVarAction()
  { }
  
  int done (SceneUser* user,Vars* vars)
  {
    Operation op;
        
    Vars* v = FamilyVar::instance().add(user->charbase.septid);
    v->set_value(op,GlobalAction<Operation>::_name,GlobalAction<Operation>::_action,GlobalAction<Operation>::_tmp,user);

    return Action::SUCCESS;
  }  
};

template <typename Operation>
class UsersVarAction : public TongVarAction<Operation>
{
public:
  typedef typename Operation::value_type value_type;
  
  UsersVarAction(ScenesParser& p) : TongVarAction<Operation>(p)
  { 
  }

  virtual ~UsersVarAction()
  { }
  
  int done (SceneUser* user,Vars* vars)
  {
    Operation op;
        
    Vars* v = UsersVar::instance().add(((QWORD)user->charbase.accid << 32) |user->charbase.id);
    v->set_value(op,GlobalAction<Operation>::_name,GlobalAction<Operation>::_action,GlobalAction<Operation>::_tmp,user);

    return Action::SUCCESS;
  }  
};

/**
 * \brief  NPC
 *
 * 该类提供了对场景上NPC访问的支持
 *
 */
class NpcAction : public Action
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  NpcAction(ScenesParser& p)
  { 
    p.value("id",_id);
    std::string map;
    p.value("map",map);
    _s = SceneManager::getInstance().getSceneByName(map.c_str());
    if (!_s) {
      Zebra::logger->warn("NpcAction时请求的场景(%s)不存在",map.c_str());
    }
    
    std::string pos;    
    p.value("ltpos",pos);  
    zRegex regex("(.*)[,](.*)");
    if (/*(regex.compile("(.*)[ ,](.*)") && */regex.match(pos.c_str())) {
      //std::string s;
      //regex.getSub(s,1);
      _ltpos.x = regex.first;//atoi(s.c_str());
      //regex.getSub(s,2);
      _ltpos.y = regex.second;//atoi(s.c_str());
    }

    p.value("rbpos",pos);  
    if (/*(regex.compile("(.*)[ ,](.*)") && */regex.match(pos.c_str())) {
      //std::string s;
      //regex.getSub(s,1);
      _rbpos.x = regex.first;//atoi(s.c_str());
      //regex.getSub(s,2);
      _rbpos.y = regex.second;//atoi(s.c_str());
    }
    
    p.value("num",_num);
    if (!_num) _num = 1;
    if (_num>100) _num = 100;
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~NpcAction()
  { }
  
  bool check_args(SceneUser* user,Vars* vars) const
  {
    return true;
  }
    
protected:
  Scene* _s;
  int _id;
  zPos _ltpos;
  zPos _rbpos;
  int _num;
  
};

/**
 * \brief 增加NPC
 *
 * 该类提供了在地图上增加一个NPC的接口
 *
 */
class AddNpcAction : public NpcAction
{
public:

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  AddNpcAction(ScenesParser& p) : NpcAction(p)
  { }
  
  /**     
   * \brief 析构函数
   *
   */     
  ~AddNpcAction()
  { }
  
  int done(SceneUser* user,Vars* vars);  
  
};

/**
 * \brief 增加镖车
 *
 * 该类提供了对护镖任务的支持
 *
 */
class AddGuardAction : public NpcAction
{
public:

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  AddGuardAction(ScenesParser& p) : NpcAction(p)
  { 
    p.value("id2",_id2);  
    p.value("id3",_id3);  
    p.value("odds1",_odds1);  
    p.value("odds2",_odds2);  
    p.value("odds3",_odds3);  
    p.value("gold",_gold);  
    p.value("path",_path);
    p.value("exp",_exp);
    p.value("script",_script);
    p.value("map",_map);

    if (_odds1+_odds2+_odds3==0)
      _odds1 = 100;

    std::string pos;    
    p.value("dest",pos);  
    zRegex regex("(.*)[,](.*)");
    if (/*(regex.compile("(.*)[ ,](.*)") &&*/ regex.match(pos.c_str())) {
      //std::string s;
      //regex.getSub(s,1);
      _dest.x = regex.first;//atoi(s.c_str());
      //regex.getSub(s,2);
      _dest.y = regex.second;//atoi(s.c_str());
      //Zebra::logger->debug("AddGuardAction::AddGuardAction(): _dest=%u,%u",_dest.x,_dest.y);
    }
    else
      Zebra::logger->info("AddGuardAction::AddGuardAction(): _dest匹配失败 %s",pos.c_str());
    
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  ~AddGuardAction()
  { }
  
  int done(SceneUser* user,Vars* vars);  

  bool check_args(SceneUser* user,Vars* vars) const
  {
    if (user) return true;
    
    return false;
  }
  
  private:
    std::string _path;
    int _gold;
    int _exp;
    int _script;
    zPos _dest;
    std::string _map;
    DWORD _id2,_id3;
    DWORD _odds1,_odds2,_odds3;
};

/**
 * \brief 删除NPC
 *
 * 该类提供了在地图上删除一个NPC的接口
 *
 */
class RemoveNpcAction : public NpcAction
{
public:

  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  RemoveNpcAction(ScenesParser& p) : NpcAction(p),_remove(_s,_id,_ltpos,_rbpos)
  { }
  
  /**     
   * \brief 析构函数
   *
   */     
  ~RemoveNpcAction()
  { }
  
  int done(SceneUser* user,Vars* vars);    
private:

  class RemoveNpc : public removeEntry_Pred<SceneNpc>
  {
  public:    
    RemoveNpc(Scene* s,int id,const zPos& ltpos,const zPos& rbpos) : _s(s),_id(id),_ltpos(ltpos),_rbpos(rbpos)
    {
    }

    bool isIt(SceneNpc *npc)
    {
      if ((int)npc->id == _id && (npc->getPos() >= _ltpos && npc->getPos() <= _rbpos) ) 
	  {
		  if( npc->npc->kind == NPC_TYPE_GHOST )   //元神特殊处理
		  {
			  SceneUser *entry = (SceneUser *)npc->getMaster();

			  if( entry )  //主人不能为空
			  {
				  BUFFER_CMD(Cmd::stAddUserAndPosMapScreenStateUserCmd,addNpc,zSocket::MAX_USERDATASIZE);
				  ((SceneGhost *)this)->full_t_MapUserDataPosState( addNpc->data, entry );
				  _s->sendCmdToNine(npc->getPosI(),&addNpc,sizeof(addNpc),npc->dupIndex);
			  }
			  else
				  return false;
		  }
		  else
		  {
			  npc->setState(zSceneEntry::SceneEntry_Hide);
			  Cmd::stAddMapNpcMapScreenUserCmd addNpc;
			  npc->full_t_MapNpcData(addNpc.data);
			  _s->sendCmdToNine(npc->getPosI(),&addNpc,sizeof(addNpc),npc->dupIndex);
		  }
        
        return true;
      }

      return false;
    }
  private:
    Scene* _s;
    int _id;
    const zPos& _ltpos;
    const zPos& _rbpos;
  };
  
  RemoveNpc _remove;
};

/**
 * \brief 变量
 *
 * 该类提供了对于任务脚本中有关五行点数的修改的支持
 *
 */  
template <typename Operation>
class FiveLevelAction : public TeamAction
{
public:
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  FiveLevelAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("value",_level);
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~FiveLevelAction()
  { }

  /**     
   * \brief  执行变量修改
   *
   * 重载了done函数,实现对相关变量的修改
   *      
   * \param user: 触发动作的用户
   * \param vars: 用户所带的该任务相关变量
   * \return SUCCESS表示成功,FAILED表示失败,DISABLE表示禁用某项功能
   */       
  int done (SceneUser* user,Vars* vars)
  {
    Operation op;
    op((int &)user->charbase.fivelevel,_level,user);

    Cmd::stMainUserDataUserCmd  userinfo;
    user->full_t_MainUserData(userinfo.data);
    user->sendCmdToMe(&userinfo,sizeof(userinfo));

    return Action::SUCCESS;
  }
  
private:
  value_type _level;
};

/**
 * \brief 设置五行类型
 *
 * 该类提供了设置五行类型的接口
 *
 */
class FiveTypeAction : public TeamAction
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  FiveTypeAction(ScenesParser& p) : TeamAction(p)
  {
    p.value("value",_type);
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~FiveTypeAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  int _type;
};

template <typename Operation>
class HonorAction : public TeamAction
{
public:
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  HonorAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("value",_honor);
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~HonorAction()
  { }

  /**     
   * \brief  执行变量修改
   *
   * 重载了done函数,实现对相关变量的修改
   *      
   * \param user: 触发动作的用户
   * \param vars: 用户所带的该任务相关变量
   * \return SUCCESS表示成功,FAILED表示失败,DISABLE表示禁用某项功能
   */       
  int done (SceneUser* user,Vars* vars)
  {
    Operation op;
    op((int &)user->charbase.honor,_honor,user);

    Cmd::stMainUserDataUserCmd  userinfo;
    user->full_t_MainUserData(userinfo.data);
    user->sendCmdToMe(&userinfo,sizeof(userinfo));

    return Action::SUCCESS;
  }
  
private:
  value_type _honor;
};

template <typename Operation>
class MaxHonorAction : public TeamAction
{
public:
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  MaxHonorAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("value",_honor);
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~MaxHonorAction()
  { }

  /**     
   * \brief  执行变量修改
   *
   * 重载了done函数,实现对相关变量的修改
   *      
   * \param user: 触发动作的用户
   * \param vars: 用户所带的该任务相关变量
   * \return SUCCESS表示成功,FAILED表示失败,DISABLE表示禁用某项功能
   */       
  int done (SceneUser* user,Vars* vars)
  {
    Operation op;
    op((int &)user->charbase.maxhonor,_honor,user);

    Cmd::stMainUserDataUserCmd  userinfo;
    user->full_t_MainUserData(userinfo.data);
    user->sendCmdToMe(&userinfo,sizeof(userinfo));

    return Action::SUCCESS;
  }
  
private:
  value_type _honor;
};


template <typename Operation>
class ActionPointAction : public TeamAction
{
public:
  typedef typename Operation::value_type value_type;
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  ActionPointAction(ScenesParser& p) : TeamAction(p)
  { 
    p.value("value",_ap);
  }

  /**     
   * \brief 析构函数
   *
   */     
  virtual ~ActionPointAction()
  { }

  /**     
   * \brief  执行变量修改
   *
   * 重载了done函数,实现对相关变量的修改
   *      
   * \param user: 触发动作的用户
   * \param vars: 用户所带的该任务相关变量
   * \return SUCCESS表示成功,FAILED表示失败,DISABLE表示禁用某项功能
   */       
  int done (SceneUser* user,Vars* vars)
  {
    Cmd::Session::t_ChangeAP  cmd;;
    cmd.id = user->charbase.unionid;
    cmd.point = _ap;
    sessionClient->sendCmd(&cmd,sizeof(cmd));
    
    return Action::SUCCESS;
  }
  
private:
  value_type _ap;
};

class UseSkillAction : public TeamAction
{
public:
  
  /**     
   * \brief  构造函数
   *
   * 初始化相关变量
   *      
   * \param p: 关键词列表
   */       
  UseSkillAction(ScenesParser& p) : TeamAction(p)
  {
    p.value("id",_id);
    p.value("level",_level);
  }
  
  /**     
   * \brief 析构函数
   *
   */     
  virtual ~UseSkillAction() { }  
  
  int done (SceneUser* user,Vars* vars);
  
private:
  int _id;
  int _level;
};

/**
 * \brief 产品工厂
 *
 * 封装了工厂模式
 *
 */
template <typename I,typename C>
class ProductFactory
{
public:
  typedef ProductFactory<I,C> self_t;
  
  /**     
   * \brief 
   *
   * 实现了单件模式
   *      
   * \return 该类的唯一实例
   */       
  static self_t& instance()
  {
    if (!_instance) {
      static self_t new_instance;
      _instance = &new_instance;
    }
    
    return *_instance;
  }

  /**     
   * \brief 取得创建类
   *
   * 寻找特定产品标志的创建类
   *      
   * \param id: 产品标识
   * \return 找的产品创建类,没找到返回NULL
   */       
  C* get_creator(const I& id) const
  {    
    const_list_iterator it = _list.find(id);
    if (it!= _list.end() ) {
      return it->second;
    }
    
    return NULL;    
  }
  
  /**     
   * \brief 注册方法
   *
   * 注册某类产品的创建类
   *      
   * \param id: 产品标识
   * \param creator: 产品创建类
   * \return 无
   */       
  void register_creator(const I& id,C* creator)  
  {
    _list[id] = creator;
  }
private:
  /**     
   * \brief 构造函数
   *
   */     
  ProductFactory()
  { }
  
  /**     
   * \brief 析构函数
   *
   */     
  ~ProductFactory()
  { }
  
  static self_t* _instance;
  
  typedef std::map<I,C*> LIST;
  typedef typename LIST::iterator list_iterator;
  typedef typename LIST::const_iterator const_list_iterator;
  LIST _list;
};

template<typename I,typename C>
ProductFactory<I,C>* ProductFactory<I,C>::_instance = NULL;

namespace MakeFunction {
  /**
   * \brief 产品创建基类
   *
   *定义了创建产品的接口
   *
   */
  template <typename A>
  class Maker
  {
  public:  
    typedef typename A::Base Base;

    /**     
     * \brief  产品创建接口
     *
     * 定义了产品创建 的接口,继承类需要实现此函数提供具体产品的创建方法.
     *      
     * \param xml: 脚本文件
     * \param node: 节点名称
     * \return 创建的产品
     */       
    virtual Base* make ( zXMLParser& xml,xmlNodePtr& node) = 0;

    /**     
     * \brief 析构函数
     *
     */     
    virtual ~Maker() { }
  };
  
  /**
   * \brief 节点内容创建类
   *
   *封装了需要访问节点内容的脚本触发条件及动作类的创建
   *
   */  
  template <typename A>
  class Content : public Maker<typename A::Base>
  {
  public:  
    typedef typename A::Base Base;
    
    /**     
     * \brief 析构函数
     *
     */     
    virtual ~Content() { }
    
    /**     
     * \brief  产品创建接口
     *
     *实现对访问节点内容的脚本触发条件及动作类的创建
     *      
     * \param xml: 脚本文件
     * \param node: 节点名称
     * \return 创建的产品
     */       
    Base* make (zXMLParser& xml,xmlNodePtr& node)
    {
      std::string content;
      if (!xml.getNodeContentStr(node,content)) {
        return NULL;
      }
      
      return new A(content);      
    }
  };
  
  /**
   * \brief 空属性创建类
   *
   *封装了不需要访问节点属性的脚本触发条件及动作类的创建
   *
   */  
  template <typename A>
  class Void : public Maker<Action>
  {
  public:  
    typedef Maker<Action>::Base Base;
    
    /**     
     * \brief  构造函数
     *
     * 初始化相关变量
     *      
     */       
    Void()
    { }
    
    /**     
     * \brief 析构函数
     *
     */     
    virtual ~Void() { }
    
    /**     
     * \brief  产品创建接口
     *
     *      
     * \param xml: 脚本文件
     * \param node: 节点名称
     * \return 创建的产品
     */       
    Base* make (zXMLParser& xml,xmlNodePtr& node)
    {
      return  new A();
    }
  };

  /**
   * \brief 节点属性创建类
   *
   *封装了需要访问节点属性的脚本触发条件及动作类的创建
   *
   */  
  template <typename A>
  class Prop : public Maker<typename A::Base>
  {
  public:  
    typedef typename A::Base Base;
    
    /**     
     * \brief  构造函数
     *
     * 初始化相关变量
     *      
     * \param p: 关键词列表
     */       
    Prop(ScenesParser& p) : _p(p)
    { }
    
    /**     
     * \brief 析构函数
     *
     */     
    virtual ~Prop() { }
    
    /**     
     * \brief  产品创建接口
     *
     *实现对访问节点属性的脚本触发条件及动作类的创建
     *      
     * \param xml: 脚本文件
     * \param node: 节点名称
     * \return 创建的产品
     */       
    Base* make (zXMLParser& xml,xmlNodePtr& node)
    {
      if (! _p.parse(xml,node) ) return NULL;

      return new A(_p);      
    }
  private:
  
    ScenesParser _p;
  };

  /**
   * \brief 触发动作创建类
   *
   *封装了需要按操作类型更新的脚本触发动作类的创建
   *
   */  
  template < template <typename> class A,typename T = int >
  class Update : public Maker<Action>
  {
  public:  
    typedef Maker<Action>::Base Base;
    
    /**     
     * \brief  构造函数
     *
     * 初始化相关变量
     *      
     * \param p: 关键词列表
     */       
    Update(ScenesParser& p) : _p(p)
    {
      
    }
    
    /**     
     * \brief 析构函数
     *
     */     
    virtual ~Update() { }
    
    /**     
     * \brief  产品创建接口
     *
     *实现对按操作类型更新的脚本触发动作类的创建
     *      
     * \param xml: 脚本文件
     * \param node: 节点名称
     * \return 创建的产品
     */       
    Base* make (zXMLParser& xml,xmlNodePtr& node)
    {
      if (! _p.parse(xml,node) ) return NULL;

      std::string type;
      _p.value("type",type);

      if (!type.compare("Set")) {
        return new A< Op::Set<T> >(_p);  
      }else if (!type.compare("Add")) {
        return new A< Op::Add<T> >(_p);                      
      }else if (!type.compare("Sub")) {
        return new A< Op::Sub<T> >(_p);                  
      }else if (!type.compare("Mul")) {
        return new A< Op::Mul<T> >(_p);                  
      }else if (!type.compare("Pow")) {
        return new A< Op::Pow<T> >(_p);                  
      }else if (!type.compare("GetP")) {
        return new A< Op::GetP<T> >(_p);                  
      }else if (!type.compare("Div")) {
        return new A< Op::Div<T> >(_p);                  
      }

      return NULL;
    }
    
    private:
      ScenesParser _p;
  };

  /**
   * \brief 触发条件创建类
   *
   *封装了需要按操作类型比较的脚本触发条件类的创建
   *
   */  
  template <template <typename> class A,typename T = int >
  class Compare : public Maker<Condition>
  {
  public:  
    typedef Maker<Condition>::Base Base;
    
    /**     
     * \brief  构造函数
     *
     * 初始化相关变量
     *      
     * \param p: 关键词列表
     */       
    Compare(ScenesParser& p) : _p(p)
    { 
      
    }
    
    /**     
     * \brief 析构函数
     *
     */     
    virtual ~Compare() { }
    
    /**     
     * \brief  产品创建接口
     *
     *实现对按操作类型比较的脚本触发条件类的创建
     *      
     * \param xml: 脚本文件
     * \param node: 节点名称
     * \return 创建的产品
     */ 
    Base* make (zXMLParser& xml,xmlNodePtr& node)
    {
      if (! _p.parse(xml,node) ) return NULL;
      
      std::string type;
      _p.value("type",type);
      
      if (!type.compare("Equal")) {
        return new A< Op::Equal<T> >(_p);
      }else if (!type.compare("Great")) {
        return new A< Op::Great<T> >(_p);                      
      }else if (!type.compare("Less")) {
        return new A< Op::Less<T> >(_p);                      
      }else if (!type.compare("Differ")) {
        return new A< Op::Differ<T> >(_p);                      
      }
      
      return NULL;
    }
    
    private:
      ScenesParser _p;
  };

}
  
typedef ProductFactory<std::string,MakeFunction::Maker<Action> > ActionFactory;
typedef ProductFactory<std::string,MakeFunction::Maker<Condition> > ConditionFactory;

/**
 * \brief 消息系统
 *
 */
class MessageSystem
{

  public:

    ~MessageSystem()
    {
      final();
    }

    /**
     * \brief 得到唯一实例
     *
     */
    static MessageSystem &getInstance()
    {
      if (NULL == instance)
        instance = new MessageSystem();

      return *instance;
    }

    /**
     * \brief 卸载唯一实例
     *
     */
    static void delInstance()
    {
      SAFE_DELETE(instance);
    }

    bool init();
    void check(SceneUser *sceneUser,const bool init = false);

  private:

    static MessageSystem *instance;

    MessageSystem() {};
    void final();

    /**
     * \brief 消息数据结构
     *
     */
    struct t_Message
    {
      time_t starttime;
      time_t endtime;
      bool login;
      WORD interval;
      int count;
      int order;
      Cmd::stChannelChatUserCmd cmd;
    };

    std::vector<t_Message> messages;
    zRWLock rwlock;

};

/**
  A script that can be executed by a virtual machine.
*/
class LuaScript 
{
public:
  LuaScript( );

  virtual ~LuaScript();

  virtual bool isLoaded();

  virtual void setData( const std::string& rData );

  const std::string& getData() const;

private:
  std::string mFileName;
};

const DWORD LUALIB_BASE    = 0x00000001;
const DWORD LUALIB_TABLE    = 0x00000002;
const DWORD LUALIB_IO      = 0x00000004;
const DWORD LUALIB_STRING    = 0x00000008;
const DWORD LUALIB_MATH    = 0x00000010;
const DWORD LUALIB_DEBUG    = 0x00000020;

struct lua_State;

/** Scripting virtual machine.
  A virtual machine can execute scripting code.
*/
class LuaVM
{
protected:
  lua_State  * mLuaState;
public:
  LuaVM( DWORD libs = LUALIB_BASE|LUALIB_TABLE|LUALIB_STRING|LUALIB_MATH|LUALIB_IO );

  virtual ~LuaVM();

  virtual void execute( const std::string & rData );

  virtual void execute( LuaScript* pScript);

  lua_State* getLuaState() const { return mLuaState; }
};

/** Scripting system interface.
  Provides means to create virtual machines and scripts.
*/
class ScriptingSystemLua 
{
public:
  static ScriptingSystemLua& instance();

  virtual LuaVM* createVM();
  
  LuaVM* getVM(int index);

  void reloadVM();

  virtual LuaScript* createScriptFromFile( const std::string & rFile );

private:
  ScriptingSystemLua();
  virtual ~ScriptingSystemLua();
  

  typedef std::vector< LuaVM* > VMList;
  VMList  mVMs;
  static ScriptingSystemLua* _instance;
  
};

/** Script binder,
  Provides means to export system interfaces to script
*/
class Binder
{
public:
  void bind( LuaVM* pVM );
};

namespace luabind
{
  namespace detail {
    template<class T>
    struct delete_s;
    template<class T>
    struct destruct_only_s;  
  }
}

/** global index,used to decide executing a script or not.
  it's for speed purpose and avoid wasting cpu time
*/
class ScriptQuest
{
public:
  enum {
    NPC_VISIT = 1,
    NPC_KILL = 2,
    OBJ_USE = 4,
    OBJ_GET = 8,
    OBJ_DROP = 16,
  };
    
  static ScriptQuest& get_instance();
  
  void add(int type,int id);
  bool has(int type,int id) const;
  
  //for speed
  void sort();

  
private:
  ScriptQuest() {}  
  ~ScriptQuest() { }

  friend class luabind::detail::delete_s<ScriptQuest>;  
  friend class luabind::detail::destruct_only_s<ScriptQuest>;  
  
  int hash(int type,int id) const;

  static ScriptQuest*   _instance;
  
  hash_set<int> _sq;
};

class SceneUser;
extern SceneUser* current_user;

inline int execute_script_event(const char* func)
{

  try {
    LuaVM* vm = ScriptingSystemLua::instance().getVM(0);
    int ret = luabind::call_function<int>(vm->getLuaState(),func);
    return ret;
  }
  catch (luabind::error& e)
  {
    Zebra::logger->debug("CATCHED Luabind EXCEPTION:%s\n%s",func,e.what());
    return 0;
  }          
  catch (const char* msg)
  {
    Zebra::logger->debug("CATCHED (...) EXCEPTION:%s\n%s",func,msg);
    return 0;
  }
  
  return 0;  
}


inline int execute_script_event(SceneUser * user,const char* func)
{
  if (user == NULL )
  {
    Zebra::logger->error("设置任务脚本当前用户空指针");
    return 0;
  }
  else
    current_user=user;
  try {
    LuaVM* vm = ScriptingSystemLua::instance().getVM(0);
    int ret = luabind::call_function<int>(vm->getLuaState(),func);
    return ret;
  }
  catch (luabind::error& e)
  {
    Zebra::logger->debug("CATCHED Luabind EXCEPTION:%s\n%s",func,e.what());
    return 0;
  }          
  catch (const char* msg)
  {
    Zebra::logger->debug("CATCHED (...) EXCEPTION:%s\n%s",func,msg);
    return 0;
  }
  
  return 0;  
}

template<typename P>
int execute_script_event(SceneUser * user,const char* func,P& p)
{
  if (user == NULL )
  {
    Zebra::logger->error("设置任务脚本当前用户空指针");
    return 0;
  }
  else
    current_user=user;
  try {
    LuaVM* vm = ScriptingSystemLua::instance().getVM(0);
    int ret = luabind::call_function<int>(vm->getLuaState(),func,p);
    return ret;
  }
  catch (luabind::error& e)
  {
    Zebra::logger->debug("CATCHED Luabind EXCEPTION:%s\n%s",func,e.what());
    return 0;
  }          
  catch (const char* msg)
  {
    Zebra::logger->debug("CATCHED (...) EXCEPTION:%s\n%s",func,msg);
    return 0;
  }
  
  return 0;  
}

template<typename P1,typename P2>
int execute_script_event(SceneUser * user,const char* func,P1& p1,P2& p2)
{
  if (user == NULL )
  {
    Zebra::logger->error("设置任务脚本当前用户空指针");
    return 0;
  }
  else
    current_user=user;
  try {
    LuaVM* vm = ScriptingSystemLua::instance().getVM(0);
    int ret = luabind::call_function<int>(vm->getLuaState(),func,p1,p2);
    return ret;
  }
  catch (luabind::error& e)
  {
    Zebra::logger->debug("CATCHED Luabind EXCEPTION:%s\n%s",func,e.what());
    return 0;
  }          
  catch (const char* msg)
  {
    Zebra::logger->debug("CATCHED (...) EXCEPTION:%s\n%s",func,msg);
    return 0;
  }
  
  return 0;  
}

template<typename P1,typename P2,typename P3>
int execute_script_event(SceneUser * user,const char* func,P1& p1,P2& p2,P3& p3)
{
  if (user == NULL )
  {
    Zebra::logger->error("设置任务脚本当前用户空指针");
    return 0;
  }
  else
    current_user=user;
//  Zebra::logger->debug("execute_script_event:%s,%s,%u,%u",func,p1->name,p2,p3);
  try {
    LuaVM* vm = ScriptingSystemLua::instance().getVM(0);
    int ret = luabind::call_function<int>(vm->getLuaState(),func,p1,p2,p3);
    return ret;
  }
  catch (luabind::error& e)
  {
    Zebra::logger->debug("CATCHED Luabind EXCEPTION:%s\n%s",func,e.what());
    return 0;
  }          
  catch (const char* msg)
  {
    Zebra::logger->debug("CATCHED (...) EXCEPTION:%s\n%s",func,msg);
    return 0;
  }
  
  return 0;  
}


template<typename P1,typename P2,typename P3>
int execute_script_event(SceneUser * user,const char* func,const P1 *p1,const P2 *p2,P3& p3)
{
  if (user == NULL )
  {
    Zebra::logger->error("设置任务脚本当前用户空指针");
    return 0;
  }
  else
    current_user=user;
//  Zebra::logger->debug("execute_script_event:%s,%s,%u,%u",func,p1->name,p2,p3);
  try {
    LuaVM* vm = ScriptingSystemLua::instance().getVM(0);
    int ret = luabind::call_function<int>(vm->getLuaState(),func,p1,p2,p3);
    return ret;
  }
  catch (luabind::error& e)
  {
    Zebra::logger->debug("CATCHED Luabind EXCEPTION:%s\n%s",func,e.what());
    return 0;
  }          
  catch (const char* msg)
  {
    Zebra::logger->debug("CATCHED (...) EXCEPTION:%s\n%s",func,msg);
    return 0;
  }
  
  return 0;  
}

SceneUser* me();
void set_me(SceneUser* user);

bool sys(SceneUser* target,int type,const char* msg);
void globalSys(SceneUser* sender,const char* msg);
void show_dialog(SceneNpc* npc,const char* menu);
void equip_make(SceneUser* user,DWORD thisid,bool drop,int flag);
zObject *CreateObject(DWORD objID,DWORD level,DWORD quantity);
zObject *getObjByTempidFromPackage(SceneUser* user,DWORD thisid);
zObject *getObjByIdFromPackage(SceneUser* user,DWORD id);
void makerName(const char *name,DWORD thisid);
char * getMeterials(DWORD thisid);
DWORD getIdByThisId(DWORD thisid);
void deleteChar(const char *p);
void infoUserObjUpdate(SceneUser *user,DWORD thisid);
void assign_set(DWORD thisid);
class Vars;

void set_var(Vars* vars,const char* name,int value);
void set_varS(Vars* vars,const char* name,const char * value);

int get_var(Vars* vars,const char* name);
const char * get_varS(Vars* vars,const char* name);

void refresh_status(SceneNpc* npc);

void refresh_npc(int id);

int npc_tempid(SceneNpc* npc);

int npc_id(SceneNpc* npc);

void refresh_quest(int id);



/** brief 提供对脚本中全局变量的支持
  
*/
class GlobalVars
{
public:
  static Vars* add_g();
  static Vars* add_t();
  static Vars* add_f();
};

bool check_money(SceneUser* user,int money);
bool remove_money(SceneUser* user,int money);
void add_money(SceneUser* user,int money);

bool have_ob(SceneUser* user,int id,int num,int level,int type);
DWORD get_ob(SceneUser* user,int id,int level );
bool del_ob(SceneUser* user,DWORD id);
int  space(SceneUser* user);

Vars * get_familyvar(SceneUser* user,int dummy);
Vars * get_uservar(SceneUser* user,int dummy);
Vars * get_tongvar(SceneUser* user,int dummy);

void add_exp(SceneUser* user,DWORD num,bool addPet,DWORD dwTempID,BYTE byType,bool addCartoon);
int get_time();
double diff_time(int,int);
bool gomap(SceneUser *pUser,const char *para);
bool goTo(SceneUser *pUser,const char *para);
bool addExp(SceneUser *pUser,const char *para);
extern NFilterModuleArray g_nFMA;

//sky 动态地图配置类
class stRangMap
{
private:
	WORD Countryid;	//sky 战场国家ID
	WORD BaseID;		//sky 原ID(根据这个来寻找地图的基本配置)
	WORD MinID;			//sky 该场景动态可分配的最小ID
	WORD MaxID;			//sky 该场景动态可分配的最大ID
	zUniqueDWORDID * MapIDManager;	//sky 唯一ID分配器

public:
	stRangMap()
	{
		Countryid = 0;
		BaseID = 0;
		MinID = 0;
		MaxID = 0;
		MapIDManager = NULL;
	}

	~stRangMap() 
	{
		if(MapIDManager)
			delete MapIDManager;
	}

	void init(WORD countryid, WORD baseid, WORD minid, WORD maxid)
	{
		Countryid = countryid;
		BaseID = baseid;
		MinID = minid;
		MaxID = maxid;
		MapIDManager = new zUniqueDWORDID(minid, maxid);
	}

	WORD GetCountryid()
	{
		return Countryid;
	}

	/**
	* \brief 生成一个唯一ID
	*/
	bool getUniqeID(DWORD &tempid)
	{
		tempid=MapIDManager->get();
		return (tempid!=MapIDManager->invalid());
	}

	/**
	* \brief 释放唯一ID
	*/
	void putUniqeID(const DWORD &tempid)
	{
		MapIDManager->put(tempid);
	}
};

//sky 动态地图配置容器
extern std::map<WORD, stRangMap*> RangMapData;

#include <zebra/sceneBattlefiel.h>

#endif

