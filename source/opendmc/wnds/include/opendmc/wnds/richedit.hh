/**************************************************************************//**
 * @file	richedit.hh
 * @brief	DmRichEdit 多功能文字編輯類別 Header
 * @date	2019-09-17
 * @date	2019-09-17
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_RICHEDIT_HH
#define	ODMC_WNDS_RICHEDIT_HH
#include "wnds.hh"
#include <richedit.h>

/**
 *	@class DmRichEdit
 *	@brief 多功能文字編輯類別
 */
class DmRichEdit : public DmWnds
{
public:
	DmRichEdit();
	virtual ~DmRichEdit();

	BOOL	AutoUrlDetect(int nType, TCHAR* szPtr = NULL) const;				// EM_AUTOURLDETECT			Enables or disables automatic detection of URLs by a rich edit control.
	BOOL	CanPasre(int nFormat) const;										// EM_CANPASTE				Determines whether a rich edit control can paste a specified clipboard format.
	BOOL	CanRedo() const;													// EM_CANREDO				Determines whether there are any actions in the control redo queue.
	void	ConvPosition() const;												// EM_CONVPOSITION			This message is not implemented.
	BOOL	DisplayBand(RECT* rcPtr) const;										// EM_DISPLAYBAND			Displays a portion of the contents of a rich edit control, as previously formatted for a device using the EM_FORMATRANGE message.
	void	ExGetSelection(CHARRANGE* crPtr) const;								// EM_EXGETSEL				Retrieves the startingand ending character positions of the selection in a rich edit control.
	void	ExLimitText(int nMaximum) const;									// EM_EXLIMITTEXT			Sets an upper limit to the amount of text the user can type or paste into a rich edit control.
	int		ExLineFromChar(int nCharIndex) const;								// EM_EXLINEFROMCHAR		Determines which line contains the specified character in a rich edit control.
	int		ExSetSelection(CHARRANGE* crPtr) const;								// EM_EXSETSEL				Selects a range of characters or COM objects in a Rich Edit control.
	int		FindText(int nMode, FINDTEXT* textPtr) const;						// EM_FINDTEXT				Finds text within a rich edit control.
	int		FindTextEx(int nMode, FINDTEXTEX* textPtr) const;					// EM_FINDTEXTEX			Finds text within a rich edit control.
	int		FindTextExW(int nMode, FINDTEXTEXW* textPtr) const;					// EM_FINDTEXTEXW			Finds Unicode text within a rich edit control.
	int		FindTextW(int nMode, FINDTEXTW* textPtr) const;						// EM_FINDTEXTW				Finds Unicode text within a rich edit control.
	// EM_FINDWORDBREAK			Finds the next word break before or after the specified character position or retrieves information about the character at that position.
	// EM_FORMATRANGE			Formats a range of text in a rich edit control for a specific device.
	// EM_GETAUTOURLDETECT		Indicates whether the auto URL detection is turned on in the rich edit control.
	// EM_GETBIDIOPTIONS		Indicates the current state of the bidirectional options in the rich edit control.
	// EM_GETCHARFORMAT			Determines the character formatting in a rich edit control.
	// EM_GETCTFMODEBIAS		Gets the Text Services Framework(TSF) mode bias values for a Rich Edit control.
	// EM_GETCTFOPENSTATUS		Determines if the TSF keyboard is open or closed.
	// EM_GETEDITSTYLE			Retrieves the current edit style flags.
	// EM_GETEVENTMASK			Retrieves the event mask for a rich edit control.The event mask specifies which notification codes the control sends to its parent window.
	// EM_GETHYPHENATEINFO		Gets information about hyphenation for a Rich Edit control.
	// EM_GETIMECOLOR			Retrieves the Input Method Editor(IME) composition color.This message is available only in Asian - language versions of the operating system.
	// EM_GETIMECOMPMODE		Gets the current IME mode for a rich edit control.
	// EM_GETIMECOMPTEXT		Gets the IME composition text.
	// EM_GETIMEMODEBIAS		Gets the IME mode bias for a Rich Edit control.
	// EM_GETIMEOPTIONS			Retrieves the current IME options.This message is available only in Asian - language versions of the operating system.
	// EM_GETIMEPROPERTY		Gets the propertyand capabilities of the IME associated with the current input locale.
	// EM_GETLANGOPTIONS		Gets a rich edit control's option settings for IME and Asian language support.
	// EM_GETOLEINTERFACE		Retrieves an IRichEditOle object that a client can use to access a rich edit control's COM functionality.
	// EM_GETOPTIONS			Retrieves rich edit control options.
	// EM_GETPAGEROTATE			Deprecated.Gets the text layout for a Rich Edit control.
	// EM_GETPARAFORMAT			Retrieves the paragraph formatting of the current selection in a rich edit control.
	// EM_GETPUNCTUATION		Gets the current punctuation characters for the rich edit control.This message is available only in Asian - language versions of the operating system.
	// EM_GETREDONAME			Retrieves the type of the next action, if any, in the rich edit control's redo queue.
	// EM_GETSCROLLPOS			Obtains the current scroll position of the edit control.
	// EM_GETSELTEXT			Retrieves the currently selected text in a rich edit control.
	// EM_GETTEXTEX				Gets all of the text from the rich edit control in any particular code base you want.
	// EM_GETTEXTLENGTHEX		Calculates text length in various ways.It is usually called before creating a buffer to receive the text from the control.
	// EM_GETTEXTMODE			Gets the current text modeand undo level of a rich edit control.
	// EM_GETTEXTRANGE			Retrieves a specified range of characters from a rich edit control.
	// EM_GETTYPOGRAPHYOPTIONS	Retrieves the current state of the typography options of a rich edit control.
	// EM_GETUNDONAME			Microsoft Rich Edit 2.0 and later: Retrieves the type of the next undo action, if any.
	
	// Microsoft Rich Edit 1.0 : This message is not supported.
	// EM_GETWORDBREAKPROCEX	Retrieves the address of the currently registered extended word - break procedure.
	// EM_GETWORDWRAPMODE		Gets the current word wrap and word - break options for the rich edit control.This message is available only in Asian - language versions of the operating system.
	// EM_GETZOOM				Gets the current zoom ratio, which is always between 1 / 64 and 64.
	// EM_HIDESELECTION			Hides or shows the selection in a rich edit control.
	// EM_ISIME					Determines whether the current input locale is an East Asian locale.
	// EM_PASTESPECIAL			Pastes a specific clipboard format in a rich edit control.
	// EM_RECONVERSION			Invokes the IME reconversion dialog box.
	// EM_REDO					Redoes the next action in the control's redo queue.
	// EM_REQUESTRESIZE			Forces a rich edit control to send an EN_REQUESTRESIZE notification code to its parent window.
	// EM_SELECTIONTYPE			Determines the selection type for a rich edit control.
	// EM_SETBIDIOPTIONS		Sets the current state of the bidirectional options in the rich edit control.
	// EM_SETBKGNDCOLOR			Sets the background color for a rich edit control.
	// EM_SETCHARFORMAT			Sets character formatting in a rich edit control.
	// EM_SETCTFMODEBIAS		Set the TSF mode bias for a Rich Edit control.
	// EM_SETCTFOPENSTATUS		Opens or closes the TSF keyboard.
	// EM_SETEDITSTYLE			Sets the current edit style flags.
	// EM_SETEVENTMASK			Sets the event mask for a rich edit control.The event mask specifies which notification codes the control sends to its parent window.
	// EM_SETFONTSIZE			Sets the font size for the selected text.
	// EM_SETHYPHENATEINFO		Sets the way a Rich Edit control does hyphenation.
	// EM_SETIMECOLOR			Sets the IME composition color.This message is available only in Asian - language versions of the operating system.
	// EM_SETIMEMODEBIAS		Sets the IME mode bias for a Rich Edit control.
	// EM_SETIMEOPTIONS			Sets the IME options.This message is available only in Asian - language versions of the operating system.
	// EM_SETLANGOPTIONS		Sets options for IME and Asian language support in a rich edit control.
	// EM_SETOLECALLBACK		Gives a rich edit control an IRichEditOleCallback object that the control uses to get OLE - related resources and information from the client.
	// EM_SETOPTIONS			Sets the options for a rich edit control.
	// EM_SETPAGEROTATE			Deprecated.Sets the text layout for a Rich Edit control.
	// EM_SETPALETTE			Changes the palette that rich edit uses for its display window.
	// EM_SETPARAFORMAT			Sets the paragraph formatting for the current selection in a rich edit control.
	// EM_SETPUNCTUATION		Sets the punctuation characters for a rich edit control.This message is available only in Asian - language versions of the operating system.
	// EM_SETSCROLLPOS			Tells the rich edit control to scroll to a particular point.
	// EM_SETTARGETDEVICE		Sets the target device and line width used for "what you see is what you get" (WYSIWYG)formatting in a rich edit control.
	// EM_SETTEXTEX				Combines the functionality of WM_SETTEXT and EM_REPLACESEL, and adds the ability to set text using a code page and to use either rich text or plain text.
	// EM_SETTEXTMODE			Sets the text mode or undo level of a rich edit control.The message fails if the control contains any text.
	// EM_SETTYPOGRAPHYOPTIONS	Sets the current state of the typography options of a rich edit control.
	// EM_SETUNDOLIMIT			Sets the maximum number of actions that can stored in the undo queue.
	// EM_SETWORDBREAKPROCEX	Sets the extended word - break procedure.
	// EM_SETWORDWRAPMODE		Sets the word - wrapping and word - breaking options for the rich edit control.This message is available only in Asian - language versions of the operating system.
	// EM_SETZOOM				Sets the zoom ratio anywhere between 1 / 64 and 64.
	// EM_SHOWSCROLLBAR			Shows or hides one of the scroll bars in the Text Host window.
	// EM_STOPGROUPTYPING		Stops the control from collecting additional typing actions into the current undo action.The control stores the next typing action, if any, into a new action in the undo queue.
	// EM_STREAMIN				Replaces the contents of a rich edit control with a stream of data provided by an application defined EditStreamCallback callback function.
	// EM_STREAMOUT				Causes a rich edit control to pass its contents to an application defined EditStreamCallback callback function.The callback function can then write the stream of data to a file or any other location that it chooses.
	
	// Notifications
	// Topic	Contents
	// EN_ALIGNLTR				Notifies a rich edit control's parent window that the paragraph direction has changed to left-to-right. A rich edit control sends this notification code in the form of a WM_COMMAND message.
	// EN_ALIGNRTL				Notifies a rich edit control's parent window that the paragraph direction changed to right-to-left. A rich edit control sends this notification code in the form of a WM_COMMAND message.
	// EN_CORRECTTEXT			Notifies a rich edit control's parent window that a SYV_CORRECT gesture occurred, giving the parent window a chance to cancel correcting the text. A rich edit control sends this notification code in the form of a WM_NOTIFY message.
	// EN_DRAGDROPDONE			Notifies a rich edit control's parent window that the drag-and-drop operation has completed. A rich edit control sends this notification code in the form of a WM_NOTIFY message.
	// EN_DROPFILES				Notifies a rich edit control's parent window that the user is attempting to drop files into the control. A rich edit control sends this notification code in the form of a WM_NOTIFY message when it receives the WM_DROPFILES message.
	// EN_IMECHANGE				Notifies a rich edit control's parent that the IME conversion status has changed. This message is available only for Asian-language versions of the operating system. A rich edit control sends this notification code in the form of a WM_COMMAND message.
	// EN_LINK					Notifies a rich edit control's parent window when the user clicks the mouse or when the mouse pointer is over text that has the CFE_LINK effect. The parent window of the control receives this notification code through a WM_NOTIFY message.
	// EN_LOWFIRTF				Notifies a rich edit control's parent window of a Rich Edit control that an unsupported Rich Text Format (RTF) keyword was received. A Rich Edit control sends this notification code in the form of a WM_NOTIFY message.
	// EN_MSGFILTER				Notifies a rich edit control's parent window of a keyboard or mouse event in the control. A rich edit control sends this notification code in the form of a WM_NOTIFY message.
	// EN_OBJECTPOSITIONS		Notifies a rich edit control's parent window when the control reads in objects. A rich edit control sends this notification code in the form of a WM_NOTIFY message.
	// EN_OLEOPFAILED			Notifies a rich edit control's parent window that a user action on a COM object has failed. A rich edit control sends this notification code in the form of a WM_NOTIFY message.
	// EN_PROTECTED				Notifies a rich edit control's parent window that the user is taking an action that would change a protected range of text. A rich edit control sends this notification code in the form of a WM_NOTIFY message.
	// EN_REQUESTRESIZE			Notifies a rich edit control's parent window that the control's contents are either smaller or larger than the control's window size. A rich edit control sends this notification code in the form of a WM_NOTIFY message.
	// EN_SAVECLIPBOARD			Notifies the rich edit control's parent window that the control is closing and the clipboard contains information. A rich edit control sends this notification code in the form of a WM_NOTIFY message.
	// EN_SELCHANGE				Notifies a rich edit control's parent window that the current selection has changed. A rich edit control sends this notification code in the form of a WM_NOTIFY message.
	// EN_STOPNOUNDO			Notifies a rich edit control's parent window that an action occurred for which the control cannot allocate enough memory to maintai

private:
	DmRichEdit(const DmRichEdit&) = delete;				//!< Disable copy construction
	DmRichEdit& operator=(const DmRichEdit&) = delete;	//!< Disable assignment operator
};

#endif // !ODMC_WNDS_RICHEDIT_HH
