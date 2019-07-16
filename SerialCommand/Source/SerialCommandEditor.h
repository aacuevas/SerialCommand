#include <EditorHeaders.h>

namespace SerialCommand {
	class SerialCommandEditor : public GenericEditor, ComboBoxListener
	{
	public:
		SerialCommandEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);
		~SerialCommandEditor();

		void comboBoxChanged(ComboBox* combo) override;

		void updateSettings() override;

		void startAcquisition() override;

		void stopAcquisition() override;

	private:
		ScopedPointer<ComboBox> m_comSelector;
		ScopedPointer<Label> m_comLabel;
		ScopedPointer<ComboBox> m_baudSelector;
		ScopedPointer<Label> m_baudLabel;
		ScopedPointer<ComboBox> m_triggerSelector;
		ScopedPointer<Label> m_triggerLabel;
		ScopedPointer<TextEditor> m_commandLine;
		ScopedPointer<Label> m_commandLabel;

		struct SelectedEvent
		{
			int eventIndex;
			int channelIndex;
		};

		Array<SelectedEvent> m_eventList;

		int16 m_oldEventSourceId{ 0 };
		int16 m_oldEventStreamIdx{ 0 };
		int16 m_oldEventIndex{ 0 };
		int16 m_oldEventChannel{ 0 };

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SerialCommandEditor);
	};
}