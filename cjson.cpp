
#include <iostream>
#include <typeinfo>
#include <map>
#include <tuple>

namespace jsonSpace
{
  class jsonValue
  {
    public:

      bool bVal;
      double nVal;
      std::string sVal;
      std::map<std::string, jsonValue*> jVal;
      bool isNull;

      int valType;
      bool isList;

      jsonValue *nextLink = NULL;
      jsonValue *prevLink = NULL;

      jsonValue *nextLink_SL = NULL;
      jsonValue *prevLink_SL = NULL;

      ~jsonValue() { }

      jsonValue() 
      { 
        this->isNull = true; 
        this->valType = -1;
        this->isList = false;
      }

      jsonValue(bool tVal)
      {
        this->bVal = tVal;
        valType = 0;
        this->isNull = false;
        this->isList = false;
      }
      jsonValue(double tVal)
      {
        this->nVal = tVal;
        valType = 1;
        this->isNull = false;
        this->isList = false;
      }
      jsonValue(std::string tVal)
      {
        this->sVal = tVal;
        valType = 2;
        this->isNull = false;
        this->isList = false;
      }
      jsonValue(std::map<std::string, jsonValue*> tVal)
      {
        this->jVal = tVal;
        valType = 3;
        this->isNull = false;
        this->isList = false;
      }

      std::size_t onGetLength()
      {
        std::size_t tLen = 1;
        jsonValue* tempLink = this;
        while(true) 
        {
          tLen += int(tempLink->nextLink_SL != NULL) + int(tempLink->nextLink != NULL);
          if (tempLink->nextLink == NULL) { break; }
          tempLink = tempLink->nextLink;
        }
        return tLen;
      }

      jsonValue* operator[] (size_t tIndex)
      {
        std::size_t cIdx = 0;
        jsonValue* outVal = this;
        while (true)
        {
          if (cIdx == tIndex) { return outVal; }
          if (outVal->nextLink_SL != NULL && cIdx+1 == tIndex) { return outVal->nextLink_SL; }
          if (outVal->nextLink == NULL) { break; }
          cIdx += int(outVal->nextLink_SL != NULL) + int(outVal->nextLink != NULL);
          outVal = outVal->nextLink;
        }
        if (cIdx <= tIndex) { throw std::overflow_error("Index " + std::to_string(tIndex) + " Is Out of Bounds For Linked List Of Size " + std::to_string(this->onGetLength()) + "."); }
        return NULL;
      }
  };

  void onPrintList(jsonValue *lVal, std::string lSpace="  ");
  void onPrintJSON(std::map<std::string, jsonValue*> tMap, std::string lSpace="  ");

  void onPrintList(jsonValue *lVal, std::string lSpace)
  {
    std::cout << lSpace + "[ " << std::endl;
    lSpace = lSpace + "  ";

    jsonValue *itVal = lVal;
    while(true)
    {
      std::string outVal = "";
      if (itVal->valType == -1) { outVal = "NULL"; }
      else if (itVal->valType == 1) { outVal = std::to_string(itVal->nVal); }
      else if (itVal->valType == 2) { outVal = '"' + itVal->sVal + '"'; }
      if (itVal->valType == 0 && itVal->bVal) { outVal = "true"; }
      else if (itVal->valType == 0 && !itVal->bVal) { outVal = "false"; }

      if (itVal->valType < 3) { std::cout << lSpace << outVal << ", " << std::endl; }
      else if (itVal->valType == 3){ onPrintJSON((itVal->jVal), lSpace + "  "); }

      if (itVal->nextLink_SL != NULL)  { onPrintList(itVal->nextLink_SL, lSpace + "  "); }

      if (itVal->nextLink == NULL) { break; }
      itVal = itVal->nextLink;
    }
    lSpace = lSpace.substr(0, lSpace.length()-2);
    std::cout << lSpace + "], " << std::endl;
  }

  void onPrintJSON(std::map<std::string, jsonValue*> tMap, std::string lSpace)
  {
    std::cout << lSpace + "{ " << std::endl;
    lSpace = lSpace + "  ";

    std::map<std::string, jsonValue*>::iterator tIter;
    for ( tIter = tMap.begin(); tIter != tMap.end(); tIter++ )
    {
      std::string tKey = tIter->first;
      jsonValue* tVal = tIter->second;
      
      std::string outVal = "";
      if (tVal->valType == -1) { outVal = "NULL"; }
      else if (tVal->valType == 1) { outVal = std::to_string(tVal->nVal); }
      else if (tVal->valType == 2) { outVal = '"' + tVal->sVal + '"'; }
      if (tVal->valType == 0 && tVal->bVal) { outVal = "true"; }
      else if (tVal->valType == 0 && !tVal->bVal) { outVal = "false"; }

      if (tVal->valType < 3 && tVal->nextLink == NULL) { std::cout << lSpace << '"' << tKey << '"' << " : " << outVal << ", " << std::endl; }
      else if (tVal->valType == 3 && tVal->nextLink == NULL)
      {
        std::cout << lSpace << '"' << tKey << '"' << " : " << std::endl;
        if (tVal->isList)
        {
          lSpace += "  ";
          std::cout << lSpace << "[" << std::endl;
        }
        onPrintJSON(tVal->jVal, lSpace + "  " ); 
        if (tVal->isList)
        {
          std::cout << lSpace << "]," << std::endl;
          lSpace = lSpace.substr(0, lSpace.length()-2);
        }
      }

      if (tVal->nextLink != NULL) 
      {
        std::cout << lSpace << '"' << tKey << '"' << " : " << std::endl;
        onPrintList(tVal, lSpace + "  ");
      }

    }
    lSpace = lSpace.substr(0, lSpace.length()-2);
    std::cout << lSpace + "}, " << std::endl;
  }

  void onFreeListMem(jsonValue *lVal);
  void onFreeJSONMem(std::map<std::string, jsonValue*> tMap);

  void onFreeListMem(jsonValue *lVal)
  {
    jsonValue *itVal = lVal;
    while(true)
    {

      if (itVal->valType == 3){ onFreeJSONMem(itVal->jVal); }

      if (itVal->nextLink_SL != NULL)  { onFreeListMem(itVal->nextLink_SL); }

      if (itVal->nextLink == NULL) { break; }
      itVal = itVal->nextLink;

      delete itVal->prevLink;
    }
  }

  void onFreeJSONMem(std::map<std::string, jsonValue*> tMap)
  {
    std::map<std::string, jsonValue*>::iterator tIter;
    for ( tIter = tMap.begin(); tIter != tMap.end(); tIter++ )
    {
      std::string tKey = tIter->first;
      jsonValue* tVal = tIter->second;
      
      if (tVal->valType == 3 && tVal->nextLink == NULL){ onPrintJSON(tVal->jVal); }
      if (tVal->nextLink != NULL) { onFreeListMem(tVal); }

      delete tMap[tKey];
    }
  }


  size_t onFindStringEnd(std::string const &tData, std::size_t stIdx, char stEnd='"' )
  {
    for(size_t i=stIdx+1; i<tData.length(); i++)
    {
      if (tData[i] == '"' && tData[i-1] != '\\') { return i; }
    }
    return 0;
  }


  std::tuple< std::map<std::string, jsonValue*>, jsonValue*, size_t> onEvaluate(std::string const &tData, bool isJson=true, std::size_t stIdx=0, std::size_t enIdx=0, bool doVerbose=false )
  {
    std::map<std::string, jsonValue*> tMap;

    if (enIdx == 0) { enIdx = tData.length(); }
    std::size_t sI = stIdx;

    if (isJson)
    {
      while (true)
      {
        // Check For End Conditions
        if ( sI >= enIdx-1 || tData[sI] == '}' || tData[sI] == '}'|| tData[sI+1] == '}') { break; }

        // Get Key
        std::size_t kS = tData.find('"', sI) + 1;
        std::size_t kE = onFindStringEnd(tData, kS);
        std::string tKey = tData.substr(kS, kE-kS);

        // Get Value Start Index
        std::size_t vS = kE+2;
        std::size_t vE = vS+1;

        if (tData[vS] == ' ') { vS += 1;}

        if (tData[vS] == '"')
        {
          // Handle String
          vE = onFindStringEnd(tData, vS);
          tMap[tKey] = new jsonValue(tData.substr(vS+1, vE-vS-1));
          if (doVerbose) { std::cout << "\n DICT Added String -> " << tKey << " : " << tData.substr(vS+1, vE-vS-1) << std::endl; }
        }
        else if (tData[vS] == '[')
        {
          // Handle List
          auto rVals = onEvaluate(tData, false, vS, 0, doVerbose);
          vE = std::get<2>(rVals);
          tMap[tKey] = std::get<1>(rVals);
          if (doVerbose) { std::cout << "\n DICT Added List -> " << tKey << " : " << tData.substr(vS, vE-vS) << std::endl; }
        }
        else if (tData[vS] == '{')
        {
          // Handle Dict
          auto rVals = onEvaluate(tData, true, vS, 0, doVerbose);
          vE = std::get<2>(rVals);
          tMap[tKey] = new jsonValue(std::get<0>(rVals));
          if (doVerbose) { std::cout << "\n DICT Added JSON -> " << tKey << " : " << tData.substr(vS, vE-vS+1) << std::endl; }
        }
        else if (tData.substr(vS, 4) == "null")
        {
          // Handle Null
          tMap[tKey] = new jsonValue();
          vE = vE + 3;
          if (doVerbose) { std::cout << "\n DICT Added NULL -> " << tKey << " : " << tData.substr(vS, vE-vS) << std::endl; }
        }
        else if (tData[vS] == 't' || tData[vS] == 'f')
        {
          // Handle Bool
          tMap[tKey] = new jsonValue(tData[vS] == 't');
          vE = ( (vE+3) * int(tData[vS] == 't') ) + ( (vE+4) * int(tData[vS] == 'f') );
          if (doVerbose) { std::cout << "\n DICT Added Bool -> " << tKey << " : " << tData.substr(vS, vE-vS) << std::endl; }
        }
        else
        {
          // Handle Number
          vE = tData.find_first_of(",}", vS+1)-1;
          tMap[tKey] = new jsonValue(stod(tData.substr(vS, vE-vS+1)));
          if (doVerbose) { std::cout << "\n DICT Added Number -> " << tKey << " : " << tData.substr(vS, vE-vS+1) << std::endl; }
        }

        if (tData[vE] == '}' || tData[vE] == ']') { vE -= 1; }
        sI = vE+1;
      }

      return std::make_tuple(tMap, new jsonValue, sI);
    }
    else
    {
      jsonValue* prevVal = new jsonValue();
      while (true)
      {

        bool isNum = false;
        size_t vS = sI+1;
        size_t vE = vS+1;

        if ( sI >= enIdx-1 || tData[sI] == ']' || tData[sI] == ']' || tData[sI+1] == ']') { break; }

        if (tData[vS] == ' ') { vS += 1;}

        if (tData[vS] == '"')
        {
          // Handle String
          vE = onFindStringEnd(tData, vS+1);
          if (doVerbose) { std::cout << "\n List Added String -> " << " : " << tData.substr(vS+1, vE-vS-1) << std::endl; }

          jsonValue* tVal = new jsonValue(tData.substr(vS+1, vE-vS-1));
          tVal->prevLink = prevVal;
          prevVal->nextLink = tVal;
          prevVal = tVal;
        }
        else if (tData[vS] == '[')
        {
          // Handle List
          auto rVals = onEvaluate(tData, false, vS, 0, doVerbose);
          vE = std::get<2>(rVals);
          if (doVerbose) { std::cout << "\n LIST Added List -> " << " : " << tData.substr(vS, vE-vS) << std::endl; }

          jsonValue *tVal = std::get<1>(rVals);
          prevVal->nextLink_SL = tVal;
          tVal->prevLink_SL = prevVal;
        }
        else if (tData[vS] == '{')
        {
          // Handle Dict
          auto rVals = onEvaluate(tData, true, vS, 0, doVerbose);
          vE = std::get<2>(rVals);
          if (doVerbose) { std::cout << "\n LIST Added Dict -> " << " : " << tData.substr(vS, vE-vS+1) << std::endl; }

          jsonValue *tVal = new jsonValue(std::get<0>(rVals));
          tVal->prevLink = prevVal;
          prevVal->nextLink = tVal;
          prevVal = tVal;
        }
        else if (tData.substr(vS, 4) == "null")
        {
          // Handle Null
          vE = vS + 3;
          if (doVerbose) { std::cout << "\n LIST Added NULL -> " << " : " << tData.substr(vS, vE-vS) << std::endl; }

          jsonValue *tVal = new jsonValue();
          tVal->prevLink = prevVal;
          prevVal->nextLink = tVal;
          prevVal = tVal;
        }
        else if (tData[vS] == 't' || tData[vS] == 'f')
        {
          // Handle Bool
          vE = ( (vE+2) * int(tData[vS] == 't') ) + ( (vE+3) * int(tData[vS] == 'f') );
          if (doVerbose) { std::cout << "\n LIST Added Bool -> " << " : " << tData.substr(vS, vE-vS) << std::endl; }

          jsonValue *tVal = new jsonValue(tData[vS] == 't');
          tVal->prevLink = prevVal;
          prevVal->nextLink = tVal;
          prevVal = tVal;
        }
        else
        {
          // Handle Numbers
          vE = tData.find_first_of(",]", vS+1)-1;
          if (doVerbose) { std::cout << "\n LIST Added Number -> " << " : " << tData.substr(vS, vE-vS+1) << std::endl; }

          jsonValue *tVal = new jsonValue(stod(tData.substr(vS, vE-vS+1)));
          tVal->prevLink = prevVal;
          prevVal->nextLink = tVal;
          prevVal = tVal;
          isNum = true;
        }

        if (!isNum && tData[vE-1] == ']') { vE-=1; }
        sI = vE+1;
      }

      while (true)
      {
        if (prevVal->prevLink == NULL) {break;}
        prevVal = prevVal->prevLink;
      }
      prevVal = prevVal->nextLink;
      prevVal->isList = true;

      return std::make_tuple(tMap, prevVal, sI+1);
    }

  }
}
