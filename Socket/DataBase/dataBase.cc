#include "dataBase.h"

std::map<std::pair<std::string, long long>, std::string>::iterator 
DataBase::FindString(const std::string &key) {
  typedef std::map<std::pair<std::string, long long>, std::string>::iterator
      Iterator;
  // std::map<std::pair<std::string, long long>, std::string>::iterator it =
  //     String_.begin();
  std::shared_ptr<Iterator> ptr(new Iterator);
  *ptr = String_.begin();
  while (*ptr != String_.end()) {
    if ((*ptr)->first.first == key) {
      break;
    } else
      (*ptr)++;
  }
  return (*ptr);
}
std::map<std::pair<std::string, long long>,
         std::map<std::string, std::string>>::iterator 
DataBase::FindHash(const std::string &key) {
  // std::map<std::pair<std::string, long long>,
  //          std::map<std::string, std::string>>::iterator it = Hash_.begin();
  typedef std::map<std::pair<std::string, long long>,
                   std::map<std::string, std::string>>::iterator Iterator;
  std::shared_ptr<Iterator> ptr(new Iterator);
  *ptr = Hash_.begin();
  while (*ptr != Hash_.end()) {
    if ((*ptr)->first.first == key) {
      break;
    } else
      (*ptr)++;
  }
  return (*ptr);
}
std::map<std::pair<std::string, long long>, std::list<std::string>>::iterator 
DataBase::FindList(const std::string &key) {
  // std::map<std::pair<std::string, long long>,
  //          std::list<std::string>>::iterator it = List_.begin();
  typedef std::map<std::pair<std::string, long long>,
                   std::list<std::string>>::iterator Iterator;
  std::shared_ptr<Iterator> ptr(new Iterator);
  *ptr = List_.begin();
  while (*ptr != List_.end()) {
    if ((*ptr)->first.first == key) {
      break;
    } else {
      size++;
      (*ptr)++;
    }
  }
  return (*ptr);
}
void DataBase::addKeySpace(int type, int encoding, const std::string &key,
                           const std::string &value, long long expiresTime) {
  type_ = type;
  encoding_ = encoding;
  key_ = key;
  value_ = value;
  setKeySpaceExpiresTime(expiresTime);
  if (type_ == DataStructure::ObjString) {
    std::map<std::pair<std::string, long long>, std::string>::iterator it =
        FindString(key);
    if (it == String_.end())
      String_.insert(make_pair(make_pair(key, expiresTime_), value_));
    else {
      it->second = value;
    }
  } else if (type == DataStructure::ObjHash) {
    std::map<std::pair<std::string, long long>,
             std::map<std::string, std::string>>::iterator it = FindHash(key);
    if (it == Hash_.end()) {
      std::map<std::string, std::string> tmp;
      tmp.insert(make_pair(key_, value_));
      Hash_.insert(make_pair(make_pair(key_, expiresTime_), tmp));
    } else {
      std::map<std::string, std::string>::iterator iter = it->second.begin();
      while (iter != it->second.end()) {
        if (iter->first == key)
          break;
        else {
          iter++;
        }
      }
      //此刻该键值肯定存在
      iter->second = value;
    }
  } else if (type == DataStructure::ObjList) {
    std::map<std::pair<std::string, long long>,std::list<std::string>>::iterator it = FindList(key);
    //如果没有找到则加入新元素　否则更新键值
    if(it == List_.end()) {
        std::list<std::string> tmp;
        tmp.push_back(value_);
        List_.insert(make_pair(make_pair(key_, expiresTime_), tmp));
    } else {
        //更新list的值
        std::list<std::string>::iterator iter = it->second.begin();
        while(size-- > 0)
            iter++;
        *iter = value;
    }
  } else {
    std::cout << "Unknown type" << std::endl;
    abort();
  }
  std::cout << "addKeySpace has been Executed" << std::endl;
}
void DataBase::delKeySpace(int type, const std::string &key) {
  if (type == DataStructure::ObjString) {
    std::map<std::pair<std::string, long long>, std::string>::iterator it =
        String_.begin();
    while (it != String_.end()) {
      if (it->first.first == key) {
        break;
      } else
        it++;
    }
    if (it == String_.end())
      std::cout << "Not Found" << std::endl;
    else {
      String_.erase(it);
    }
  } else if (type == DataStructure::ObjHash) {
    std::map<std::pair<std::string, long long>,
             std::map<std::string, std::string>>::iterator it = Hash_.begin();
    while (it != Hash_.end()) {
      if (it->first.first == key)
        break;
      else {
        it++;
      }
    }
    if (it == Hash_.end()) {
      std::cout << "Not Found" << std::endl;
      abort();
    } else {
      Hash_.erase(it);
    }
  } else if (type == DataStructure::ObjList) {
    std::map<std::pair<std::string, long long>,
             std::list<std::string>>::iterator it = List_.begin();
    while (it != List_.end()) {
      if (it->first.first == key)
        break;
      else {
        it++;
      }
    }
    if (it == List_.end()) {
      std::cout << "Not Found" << std::endl;
      abort();
    } else
      List_.erase(it);
  } else {
    std::cout << "Unknown type" << std::endl;
    abort();
  }
}
std::string DataBase::getKeySpace(int type, const std::string &key) {
  std::string ret = std::string();
  if (!judgeKeySpaceExpiresTime(type, key)) {
    if (type == DataStructure::ObjString) {
      std::map<std::pair<std::string, long long>, std::string>::iterator it =
          String_.begin();
      while (it != String_.end()) {
        if (it->first.first == key) {
          break;
        } else
          it++;
      }
      if (it == String_.end()) ret = "Not Found";
      // std::cout << "Not Found" << std::endl;
      else
        ret = it->second;
    } else if (type == DataStructure::ObjHash) {
      std::map<std::pair<std::string, long long>,
               std::map<std::string, std::string>>::iterator it = Hash_.begin();
      while (it != Hash_.end()) {
        if (it->first.first == key)
          break;
        else {
          it++;
        }
      }
      if (it == Hash_.end()) {
        std::cout << "Not Found" << std::endl;
        ret = "Not Found";
      } else {
        std::map<std::string, std::string>::iterator iter =
            it->second.find(key);
        ret = iter->second;
      }
    } else if (type == DataStructure::ObjList) {
      std::map<std::pair<std::string, long long>,
               std::list<std::string>>::iterator it = List_.begin();
      while (it != List_.end()) {
        if (it->first.first == key)
          break;
        else {
          it++;
        }
      }
      if (it == List_.end()) {
        std::cout << "Not Found" << std::endl;
        ret = "Not Found";
      } else {
        std::list<std::string>::iterator iter = it->second.begin();
        ret = *iter;
      }
    } else {
      std::cout << "Unknown Type" << std::endl;
      ret = "Not Found";
    }
  } else {
    delKeySpace(type, key);
    ret = "The key has expired and has been deleted";
  }
  //   std::cout << "getKeySpace: " << ret << std::endl;
  return ret;
}

long long DataBase::getKeySpaceExpiresTime(int type, const std::string &key) {
  long long ret;
  if (type == DataStructure::ObjString) {
    std::map<std::pair<std::string, long long>, std::string>::iterator it =
        String_.begin();
    while (it != String_.end()) {
      if (it->first.first == key)
        break;
      else
        it++;
    }
    if (it == String_.end()) {
      ret = -2;
    } else
      ret = it->first.second;
  } else if (type == DataStructure::ObjHash) {
    std::map<std::pair<std::string, long long>,
             std::map<std::string, std::string>>::iterator it = Hash_.begin();
    while (it != Hash_.end()) {
      if (it->first.first == key)
        break;
      else
        it++;
    }
    if (it == Hash_.end())
      ret = -2;
    else
      ret = it->first.second;
  } else if (type == DataStructure::ObjList) {
    std::map<std::pair<std::string, long long>,
             std::list<std::string>>::iterator it = List_.begin();
    while (it != List_.end()) {
      if (it->first.first == key)
        break;
      else
        it++;
    }
    if (it == List_.end())
      ret = -2;
    else
      ret = it->first.second;
  } else {
    ret = -3;
  }
  return ret;
}

long long DataBase::RemainingSurvivalTime(int type, const std::string &key) {
  long long ret = getKeySpaceExpiresTime(type, key);
  assert(ret != -1);
  long long now = getTimestamp();
  return now - ret;
}
bool DataBase::judgeKeySpaceExpiresTime(int type, const std::string &key) {
  long long expire = getKeySpaceExpiresTime(type, key);
  if (expire <= -1)
    return false;
  else {
    long long now = getTimestamp();
    if (now > expire)
      return true;
    else
      return false;
  }
}