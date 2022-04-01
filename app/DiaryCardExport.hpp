#ifndef DIARYCARDEXPORT_HPP
#define DIARYCARDEXPORT_HPP

#include <QObject>

#include "app/SQLBase.hpp"
#include "app/DiaryCardEngine.hpp"


namespace diaryth {


    class DiaryCardExport : public QObject
    {
        Q_OBJECT

    public:
        explicit DiaryCardExport(const SQLBase& database,
                                 const DiaryCardEngine& cardEngine);



    private:

        const SQLBase& _database;
        const DiaryCardEngine& _cardEngine;

    };

}



#endif // DIARYCARDEXPORT_HPP
