#ifndef MELAMPIG_EXPORTS_H
#define MELAMPIG_EXPORTS_H

#ifdef Q_WS_WIN
#   ifdef EXPORT_MELAMPIG_ORM
#       define MELAMPIG_ORM_EXPORT __declspec(dllexport)
#   elif IMPORT_MELAMPIG_ORM
#       define MELAMPIG_ORM_EXPORT __declspec(dllimport)
#   else
#       define MELAMPIG_ORM_EXPORT
#   endif
#else
# define MELAMPIG_ORM_EXPORT
#endif

#ifdef Q_WS_WIN
#   ifdef EXPORT_MELAMPIG_UI
#       define MELAMPIG_UI_EXPORT __declspec(dllexport)
#   elif IMPORT_MELAMPIG_UI
#       define MELAMPIG_UI_EXPORT __declspec(dllimport)
#   else
#       define MELAMPIG_UI_EXPORT
#   endif
#else
# define MELAMPIG_UI_EXPORT
#endif

#endif // CMSORMEXPORT_H
