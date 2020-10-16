
namespace sdk
{

	namespace vgui
	{
		typedef unsigned long HFont;
		typedef unsigned int VPANEL;
	};

	enum FontFeature
	{
		FONT_FEATURE_ANTIALIASED_FONTS = 1,
		FONT_FEATURE_DROPSHADOW_FONTS = 2,
		FONT_FEATURE_OUTLINE_FONTS = 6,
	};

	enum FontDrawType
	{
		FONT_DRAW_DEFAULT = 0,
		FONT_DRAW_NONADDITIVE,
		FONT_DRAW_ADDITIVE,
		FONT_DRAW_TYPE_COUNT = 2,
	};

	enum FontFlags
	{
		FONTFLAG_NONE,
		FONTFLAG_ITALIC = 0x001,
		FONTFLAG_UNDERLINE = 0x002,
		FONTFLAG_STRIKEOUT = 0x004,
		FONTFLAG_SYMBOL = 0x008,
		FONTFLAG_ANTIALIAS = 0x010,
		FONTFLAG_GAUSSIANBLUR = 0x020,
		FONTFLAG_ROTARY = 0x040,
		FONTFLAG_DROPSHADOW = 0x080,
		FONTFLAG_ADDITIVE = 0x100,
		FONTFLAG_OUTLINE = 0x200,
		FONTFLAG_CUSTOM = 0x400,
		FONTFLAG_BITMAP = 0x800,
	};

	struct IntRect
	{
		int x0;
		int y0;
		int x1;
		int y1;
	};

	struct Vertex_t
	{
		Vertex_t() {}
		Vertex_t(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
		{
			m_Position = pos;
			m_TexCoord = coord;
		}
		void Init(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
		{
			m_Position = pos;
			m_TexCoord = coord;
		}

		Vector2D m_Position;
		Vector2D m_TexCoord;
	};


	class ISurface
	{
	public:
		void DrawSetColor(int r, int g, int b, int a)
		{
			typedef void(__thiscall* DrawSetColorFn)(void*, int, int, int, int);
			get_vfunc< DrawSetColorFn >(this, 15)(this, r, g, b, a);
		}

		void DrawFilledRect(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* DrawFilledRectFn)(void*, int, int, int, int);
			get_vfunc< DrawFilledRectFn >(this, 16)(this, x0, y0, x1, y1);
		}

		void DrawOutlinedRect(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* DrawOutlinedFn)(void*, int, int, int, int);
			get_vfunc< DrawOutlinedFn >(this, 18)(this, x0, y0, x1, y1);
		}

		void DrawLine(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* DrawLineFn)(void*, int, int, int, int);
			get_vfunc< DrawLineFn >(this, 19)(this, x0, y0, x1, y1);
		}

		void DrawSetTextFont(unsigned long font)
		{
			typedef void(__thiscall* DrawSetTextFontFn)(void*, unsigned long);
			get_vfunc< DrawSetTextFontFn >(this, 23)(this, font);
		}

		void DrawSetTextColor(Color c)
		{
			typedef void(__thiscall* DrawSetTextColorFn)(void*, Color);
			get_vfunc< DrawSetTextColorFn >(this, 25)(this, c);
		}

		void DrawSetTextPos(int x, int y)
		{
			typedef void(__thiscall* DrawSetTextPosFn)(void*, int, int);
			get_vfunc< DrawSetTextPosFn >(this, 26)(this, x, y);
		}

		void DrawPrintText(const wchar_t* text, int textLen)
		{
			typedef void(__thiscall* DrawPrintTextFn)(void*, const wchar_t*, int, int);
			return get_vfunc< DrawPrintTextFn >(this, 28)(this, text, textLen, 0);
		}

		unsigned long CreateFont_()
		{
			typedef unsigned int(__thiscall* CreateFontFn)(void*);
			return get_vfunc< CreateFontFn >(this, 71)(this);
		}

		void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
		{
			typedef void(__thiscall* SetFontGlyphSetFn)(void*, unsigned long, const char*, int, int, int, int, int, int, int);
			get_vfunc< SetFontGlyphSetFn >(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
		}

		void UnlockCursor()
		{
			typedef void(__thiscall* UnlockCursorFn)(void*);
			get_vfunc< UnlockCursorFn >(this, 66)(this);
		}

		void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall)
		{
			typedef void(__thiscall* GetTextSizeFn)(void*, unsigned long, const wchar_t*, int&, int&);
			get_vfunc< GetTextSizeFn >(this, 79)(this, font, text, wide, tall);
		}
	};


	typedef int TABLEID;

#define INVALID_STRING_TABLE -1

	class INetworkStringTable;

	typedef void(*pfnStringChanged)(void* object, INetworkStringTable* stringTable, int stringNumber, char const* newString, void const* newData);

	class INetworkStringTable
	{
	public:

		virtual					~INetworkStringTable(void) {};

		// Table Info
		virtual const char* GetTableName(void) const = 0;
		virtual TABLEID			GetTableId(void) const = 0;
		virtual int				GetNumStrings(void) const = 0;
		virtual int				GetMaxStrings(void) const = 0;
		virtual int				GetEntryBits(void) const = 0;

		// Networking
		virtual void			SetTick(int tick) = 0;
		virtual bool			ChangedSinceTick(int tick) const = 0;

		// Accessors (length -1 means don't change user data if string already exits)
		virtual int				AddString(bool bIsServer, const char* value, int length = -1, const void* userdata = 0) = 0;

		virtual const char* GetString(int stringNumber) = 0;
		virtual void			SetStringUserData(int stringNumber, int length, const void* userdata) = 0;
		virtual const void* GetStringUserData(int stringNumber, int* length) = 0;
		virtual int				FindStringIndex(char const* string) = 0; // returns INVALID_STRING_INDEX if not found

																		 // Callbacks
		virtual void			SetStringChangedCallback(void* object, pfnStringChanged changeFunc) = 0;
	};

	class INetworkStringTableContainer
	{
	public:

		virtual					~INetworkStringTableContainer(void) {};

		// table creation/destruction
		virtual INetworkStringTable* CreateStringTable(const char* tableName, int maxentries, int userdatafixedsize = 0, int userdatanetworkbits = 0) = 0;
		virtual void				RemoveAllTables(void) = 0;

		// table infos
		virtual INetworkStringTable* FindTable(const char* tableName) const = 0;
		virtual INetworkStringTable* GetTable(TABLEID stringTable) const = 0;
		virtual int					GetNumTables(void) const = 0;

		virtual INetworkStringTable* CreateStringTableEx(const char* tableName, int maxentries, int userdatafixedsize = 0, int userdatanetworkbits = 0, bool bIsFilenames = false) = 0;
		virtual void				SetAllowClientSideAddString(INetworkStringTable* table, bool bAllowClientSideAddString) = 0;
	};

}