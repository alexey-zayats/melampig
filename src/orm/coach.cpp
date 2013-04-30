#include "coach.h"

namespace Melampig
{
    Coach::Coach(Keeper *keeper) : Object(keeper)
    {
        init();
    }

    Coach::Coach( int id, Keeper *keeper) : Object(keeper)
    {
        init();
        restore( id );
    }

    Coach::Coach ( const TQueryMap &opts,Keeper *keeper) : Object(keeper)
    {
        init();
        restore( opts );
    }

    void Coach::init()
    {
        table = QString("coach");
        type = OCoach;
        mtype = aTable;

        Object::setObjectName("Melampig::Coach");

        //		Name		Name			Title			ViewType	ro	visible
        schema.append( new Attr( "title",        tr("Title"),           Attr::String,    true, true ) );

        schema.append( new Attr( "lastname",       tr("Lastname"),      Attr::String,    false, true ) );
        schema.append( new Attr( "firstname",      tr("Firstname"),     Attr::String,    false, true ) );
        schema.append( new Attr( "patronymic",     tr("Patronymic"),    Attr::String,    false, true ) );

        schema.append( new Attr( "rang",           tr("Rang"),          Attr::ObjectRef, false, true, ORang ) );
        schema.append( new Attr( "birthdate",      tr("birthdate"),     Attr::Date,      false, true ) );

        schema.append( new Attr( "gender",         tr("gender"),           Attr::ObjectRef, false, true, OGender ) );
        schema.append( new Attr( "phone",          tr("Phone"),         Attr::String,    false, true ) );
        schema.append( new Attr( "email",          tr("Email"),         Attr::String,    false, true ) );

        schema.append( new Attr( "team",           tr("Team"),          Attr::ObjectRef, false, true, OTeam ) );
        schema.append( new Attr( "picture",        tr("Picture"),       Attr::Image,    false, true ) );

        viewItems.append( "lastname" );
        viewItems.append( "firstname" );
        viewItems.append( "patronymic" );
        viewItems.append( "rang" );
        viewItems.append( "gender" );
        viewItems.append( "birthdate" );
        viewItems.append( "team" );
    }
}
