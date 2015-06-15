#include "RaveBase/RaveEngine/interface/RaveParameterSetBuilder.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <boost/spirit/home/classic/core.hpp>
#include <boost/spirit/home/classic/actor/push_back_actor.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

using namespace std;
using namespace boost::spirit::classic;

namespace {

  struct ParameterSetNode
  {
    ParameterSetNode() {};
    ~ParameterSetNode() {};
    boost::shared_ptr<edm::ParameterSet> current;
    boost::shared_ptr<ParameterSetNode> parent;
    std::string key_name;
  };
  boost::shared_ptr<ParameterSetNode> tree;
  std::string last_key;

  void nest_in_action(char ch)
  {
    //depth++;
    boost::shared_ptr<ParameterSetNode> node( new ParameterSetNode() );
    node->parent = tree;
    node->current = boost::shared_ptr<edm::ParameterSet>( new edm::ParameterSet() );
    node->key_name = last_key;
    tree = node;
    last_key = "finder";
  }
  void nest_out_action(char ch)
  {
    tree->parent->current->addParameter( tree->key_name, *(tree->current) );
    tree = tree->parent;
  }
  void key_action(char const* first, char const* last)
  {
    if ( last[0]=='(' )
    {
      edm::LogWarning("RaveParameterSetBuilder")
        << "received a weird key " << last 
        << " -- maybe you forgot a ``:'' before the ``(''?";
    }
    std::string str(first, last);
    last_key = str;
  }
  void int_action(int value)
  {
    tree->current->addParameter<int>( last_key, value );
  }
  void real_action(float value)
  {
    tree->current->addParameter<float>( last_key, value );
  }
  void str_action(char const* first, char const* last)
  {
    std::string str(first, last);
    tree->current->addParameter<std::string>( last_key, str );
  }
}

edm::ParameterSet rave::ParameterSetBuilder::create ( string s )
{
  rule<> key = (+alpha_p)[&key_action];
  rule<> value = ( ( real_p - int_p ) & real_p[&real_action] ) | int_p[&int_action] | (+alpha_p)[&str_action];
  rule<> expression;
  rule<> nestedvalue = value | ch_p('(')[&nest_in_action] >> expression >> ch_p(')')[&nest_out_action];
  expression = !( ( (+alpha_p) >> ch_p(':') ) & ( key >> ch_p(':') ) ) >> nestedvalue
               >> *( ch_p('-') >> key >> ch_p(':') >> nestedvalue );

  tree = boost::shared_ptr<ParameterSetNode>( new ParameterSetNode() );
  tree->current = boost::shared_ptr<edm::ParameterSet>( new edm::ParameterSet() );
  last_key = "finder";
  parse_info<> pI = parse(s.c_str(), expression);

  edm::ParameterSet result = *(tree->current);
  tree.reset();

  return result;
}
