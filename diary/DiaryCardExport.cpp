#include "DiaryCardExport.hpp"

using namespace diaryth;

DiaryCardExport::DiaryCardExport(const SQLBase &database,
                                 const DiaryCardEngine &cardEngine)
    :_database(database),
    _cardEngine(cardEngine)
{
}
