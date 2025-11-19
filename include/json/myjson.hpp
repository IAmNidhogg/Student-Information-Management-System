#ifndef MYJSON_HEADER
#define MYJSON_HEADER

#include <cstddef>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

enum class JSONType : short { STRING, NUMBER, BOOL, NULLT, ARRAY, OBJECT };

class JSONNode {
public:
  JSONType _type;                                  // 节点类型
  std::unordered_map<std::string, JSONNode> _data; // 如果是JSON节点则有后续指向
  std::vector<JSONNode> _array; // 如果是array，用vector存储其内容

  struct value { // 如果是value，则使用联合体存储其值
    std::string _string;
    double _dnumber; //_numbertype = 1
    int _inumber;    //_numbertype = 2
    bool _bool;
    int _numbertype = 0;
    value() {}
    ~value() {}

    operator std::string() { return _string; }
    operator double() { return _dnumber; }
    operator int() { return _inumber; }
    operator bool() { return _bool; }
  } _value;

  // 错误操作提示
  void limitToarray() {
    if (!isArray()) {
      throw std::runtime_error(
          "This operation is only available to array node");
    }
  }
  void limitToobject() {
    if (!isObject()) {
      throw std::runtime_error(
          "This operation is only available to object node");
    }
  }

public:
  // 构造函数
  JSONNode() : _type(JSONType::OBJECT) {} // 默认构造，构造一个存储null的节点
  JSONNode(JSONType type) : _type(type) {}
  JSONNode(nullptr_t value) : _type(JSONType::NULLT) {} //?
  JSONNode(const std::string &value) : _type(JSONType::STRING) {
    _value._string = value;
  }
  JSONNode(const char *value) : _type(JSONType::STRING) {
    _value._string = value;
  }
  JSONNode(double value) : _type(JSONType::NUMBER) {
    _value._dnumber = value;
    _value._numbertype = 1;
  }
  JSONNode(int value) : _type(JSONType::NUMBER) {
    _value._inumber = value;
    _value._numbertype = 2;
  }
  JSONNode(bool value) : _type(JSONType::BOOL) { _value._bool = value; }
  JSONNode(const std::vector<JSONNode> &nodes)
      : _type(JSONType::ARRAY), _array(nodes) {}

  // 拷贝构造
  JSONNode(const JSONNode &node) {
    _type = node._type;
    _data = node._data;
    _array = node._array;
    _value._string = node._value._string;
    _value._dnumber = node._value._dnumber;
    _value._inumber = node._value._inumber;
    _value._numbertype = node._value._numbertype;
    _value._bool = node._value._bool;
  }

  // 拷贝运算符
  JSONNode &operator=(const JSONNode &node) {
    _type = node._type;
    _data = node._data;
    _array = node._array;
    _value._string = node._value._string;
    _value._dnumber = node._value._dnumber;
    _value._inumber = node._value._inumber;
    _value._numbertype = node._value._numbertype;
    _value._bool = node._value._bool;
    return *this;
  }

  bool isValue() {
    return _type == JSONType::STRING || _type == JSONType::NUMBER ||
           _type == JSONType::BOOL || _type == JSONType::NULLT;
  }

  bool isNULL() { return _type == JSONType::NULLT; }

  bool isArray() { return _type == JSONType::ARRAY; }

  bool isObject() { return _type == JSONType::OBJECT; }

  void appendArray(const JSONNode &node) { _array.push_back(node); }

  template <typename T> T get() {
    if (!isValue()) {
      throw std::runtime_error("Unable to get value for this type");
    }
    return static_cast<T>(_value);
  }

  JSONNode &operator[](const std::string &key) {
    limitToobject();
    return _data[key];
  }
  JSONNode &operator[](const char *key) {
    limitToobject();
    return _data[key];
  }
  JSONNode &operator[](int index) {
    limitToarray();
    return _array[index];
  }

  operator std::string() { return _value._string; }
  operator double() { return _value._dnumber; }
  operator int() { return _value._inumber; }
  operator bool() { return _value._bool; }
};

using JSON = JSONNode;

#endif
