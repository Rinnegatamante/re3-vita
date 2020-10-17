#include "common.h"
#include "crossplatform.h"

// Codes compatible with Windows and Linux
#ifndef _WIN32

// For internal use
// wMilliseconds is not needed
void tmToSystemTime(const tm *tm, SYSTEMTIME *out) {
    out->wYear = tm->tm_year + 1900;
    out->wMonth = tm->tm_mon + 1;
    out->wDayOfWeek = tm->tm_wday;
    out->wDay = tm->tm_mday;
    out->wHour = tm->tm_hour;
    out->wMinute = tm->tm_min;
    out->wSecond = tm->tm_sec;
}

void GetLocalTime_CP(SYSTEMTIME *out) {
    time_t timestamp = time(nil);
    tm *localTm = localtime(&timestamp);
    tmToSystemTime(localTm, out);
}
#endif

// Compatible with Linux/POSIX and MinGW on Windows
#ifndef _WIN32
HANDLE FindFirstFile(const char* pathname, WIN32_FIND_DATA* firstfile) {
	char newpathname[64];
	snprintf(newpathname, sizeof(newpathname), "ux0:data/gta3/%s", pathname);
	char* path = strtok(newpathname, "\\*");
	strncpy(firstfile->folder, path, sizeof(firstfile->folder));

	// Both w/ extension and w/o extension is ok
	if (strlen(path) + 2 != strlen(pathname))
		strncpy(firstfile->extension, strtok(NULL, "\\*"), sizeof(firstfile->extension));
	else
		strncpy(firstfile->extension, "", sizeof(firstfile->extension));

	HANDLE d;
	if ((d = (HANDLE)opendir(path)) == NULL || !FindNextFile(d, firstfile))
		return NULL;

	return d;
}

#define NAME_MAX 260

bool FindNextFile(HANDLE d, WIN32_FIND_DATA* finddata) {
	dirent *file;
	static struct stat fileStats;
	static char path[PATH_MAX], relativepath[NAME_MAX + sizeof(finddata->folder) + 1];
	int extensionLen = strlen(finddata->extension);
	while ((file = readdir((DIR*)d)) != NULL) {

		if ((extensionLen == 0 || strncmp(&file->d_name[strlen(file->d_name) - extensionLen], finddata->extension, extensionLen) == 0)) {

			sprintf(relativepath, "%s/%s", finddata->folder, file->d_name);
			// realpath(relativepath, path);
			strcpy(path, relativepath);
			debug("FindNextFile %s\n", path);
			stat(path, &fileStats);
			strncpy(finddata->cFileName, file->d_name, sizeof(finddata->cFileName));
			finddata->ftLastWriteTime = fileStats.st_mtime;
			return true;
		}
	}
	return false;
}

void GetDateFormat(int unused1, int unused2, SYSTEMTIME* in, int unused3, char* out, int size) {
	tm linuxTime;
	linuxTime.tm_year = in->wYear - 1900;
	linuxTime.tm_mon = in->wMonth - 1;
	linuxTime.tm_wday = in->wDayOfWeek;
	linuxTime.tm_mday = in->wDay;
	linuxTime.tm_hour = in->wHour;
	linuxTime.tm_min = in->wMinute;
	linuxTime.tm_sec = in->wSecond;
	strftime(out, size, nl_langinfo(D_FMT), &linuxTime);
}

void FileTimeToSystemTime(time_t* writeTime, SYSTEMTIME* out) {
	tm *ptm = gmtime(writeTime);
	tmToSystemTime(ptm, out);
}
#endif

// Funcs/features from Windows that we need on other platforms
#ifndef _WIN32
char *strupr(char *s) {
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = toupper((unsigned char) *tmp);
    }

    return s;
}
char *strlwr(char *s) {
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = tolower((unsigned char) *tmp);
    }

    return s;
}

char *trim(char *s) {
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
}

FILE* _fcaseopen(char const* filename, char const* mode)
{
    FILE* result;
    char* real = casepath(filename);
    if (!real)
        result = fopen(filename, mode);
    else {
        result = fopen(real, mode);
        free(real);
    }
    return result;
}

// Case-insensitivity on linux (from https://github.com/OneSadCookie/fcaseopen)
// Returned string should freed manually (if exists)
char* casepath(char const* path, bool checkPathFirst)
{
		size_t l = strlen(path);
		char data_path[512];
		getcwd(data_path, sizeof(data_path));
		char* out = (char*)malloc(l + strlen(data_path) + 2);

		if (strncmp(path, "ux0:", 4) == 0) {
			strcpy(out, path);
		} else {
			sprintf(out, "%s/%s", data_path, path);
		}

		l = strlen(out);
		for (int i = l-1; i >= 0; i--) {
			if (out[i] == '\\' || out[i] == '/' || out[i] == ' ')
				out[i] = '\0';
			else
				break;
		}

		debug("casepath %s -> %s\n", path, out);
		return out;
}
#endif
