#include <json/json.h>
#include <string>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

using namespace google::protobuf;
using std::string;

string pb2json(const Message *msg);
string pb2json(Message *msg, const char *buf ,int len);

static bool parse_msg(const Message& msg, Json::Value& root);
static bool parse_repeated_field(const Message& msg, const Reflection& ref, const FieldDescriptor* field, Json::Value& root);
