/****************************************************************************
** My_Edit meta object code from reading C++ file 'editor.h'
**
** Created: Wed Jan 4 18:44:16 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "editor.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *My_Edit::className() const
{
    return "My_Edit";
}

QMetaObject *My_Edit::metaObj = 0;
static QMetaObjectCleanUp cleanUp_My_Edit( "My_Edit", &My_Edit::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString My_Edit::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "My_Edit", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString My_Edit::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "My_Edit", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* My_Edit::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMultiLineEdit::staticMetaObject();
    static const QUMethod signal_0 = {"cursorMove", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "cursorMove()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"My_Edit", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_My_Edit.setMetaObject( metaObj );
    return metaObj;
}

void* My_Edit::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "My_Edit" ) )
	return this;
    return QMultiLineEdit::qt_cast( clname );
}

// SIGNAL cursorMove
void My_Edit::cursorMove()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool My_Edit::qt_invoke( int _id, QUObject* _o )
{
    return QMultiLineEdit::qt_invoke(_id,_o);
}

bool My_Edit::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: cursorMove(); break;
    default:
	return QMultiLineEdit::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool My_Edit::qt_property( int id, int f, QVariant* v)
{
    return QMultiLineEdit::qt_property( id, f, v);
}

bool My_Edit::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *Editor::className() const
{
    return "Editor";
}

QMetaObject *Editor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Editor( "Editor", &Editor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Editor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Editor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Editor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Editor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Editor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"load", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "fileName", &static_QUType_charstar, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"load", 1, param_slot_1 };
    static const QUMethod slot_2 = {"save", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "fileName", &static_QUType_charstar, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"save", 1, param_slot_3 };
    static const QUMethod slot_4 = {"save_as", 0, 0 };
    static const QUMethod slot_5 = {"create", 0, 0 };
    static const QUMethod slot_6 = {"props", 0, 0 };
    static const QUMethod slot_7 = {"print", 0, 0 };
    static const QUMethod slot_8 = {"cmp", 0, 0 };
    static const QUMethod slot_9 = {"gen", 0, 0 };
    static const QUMethod slot_10 = {"comp_all", 0, 0 };
    static const QUMethod slot_11 = {"log_unit", 0, 0 };
    static const QUMethod slot_12 = {"log_prog", 0, 0 };
    static const QUMethod slot_13 = {"findText", 0, 0 };
    static const QUMethod slot_14 = {"find_next", 0, 0 };
    static const QUMethod slot_15 = {"updateline", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "load()", &slot_0, QMetaData::Public },
	{ "load(const char*)", &slot_1, QMetaData::Public },
	{ "save()", &slot_2, QMetaData::Public },
	{ "save(const char*)", &slot_3, QMetaData::Public },
	{ "save_as()", &slot_4, QMetaData::Public },
	{ "create()", &slot_5, QMetaData::Public },
	{ "props()", &slot_6, QMetaData::Public },
	{ "print()", &slot_7, QMetaData::Public },
	{ "cmp()", &slot_8, QMetaData::Public },
	{ "gen()", &slot_9, QMetaData::Public },
	{ "comp_all()", &slot_10, QMetaData::Public },
	{ "log_unit()", &slot_11, QMetaData::Public },
	{ "log_prog()", &slot_12, QMetaData::Public },
	{ "findText()", &slot_13, QMetaData::Public },
	{ "find_next()", &slot_14, QMetaData::Public },
	{ "updateline()", &slot_15, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Editor", parentObject,
	slot_tbl, 16,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Editor.setMetaObject( metaObj );
    return metaObj;
}

void* Editor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Editor" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool Editor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: load(); break;
    case 1: load((const char*)static_QUType_charstar.get(_o+1)); break;
    case 2: save(); break;
    case 3: save((const char*)static_QUType_charstar.get(_o+1)); break;
    case 4: save_as(); break;
    case 5: create(); break;
    case 6: props(); break;
    case 7: print(); break;
    case 8: cmp(); break;
    case 9: gen(); break;
    case 10: comp_all(); break;
    case 11: log_unit(); break;
    case 12: log_prog(); break;
    case 13: findText(); break;
    case 14: find_next(); break;
    case 15: updateline(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Editor::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Editor::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool Editor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
