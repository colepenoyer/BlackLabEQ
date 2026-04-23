/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BlackLabEQAudioProcessorEditor::BlackLabEQAudioProcessorEditor (BlackLabEQAudioProcessor& p)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
      lowCutFreqAttachment(audioProcessor.apvts, "LowCutFreq", lowCutFreqSlider),
      peakFreqAttachment(audioProcessor.apvts, "PeakFreq", peakFreqSlider),
      peakGainAttachment(audioProcessor.apvts, "PeakGain", peakGainSlider),
      peakQualityAttachment(audioProcessor.apvts, "PeakQuality", peakQualitySlider),
      highCutFreqAttachment(audioProcessor.apvts, "HighCutFreq", highCutFreqSlider)
{
    for (auto* comp : getComps())
    {
        addAndMakeVisible(comp);
    }

    lowCutLabel.setText("LOW CUT", juce::dontSendNotification);
    lowCutLabel.setJustificationType(juce::Justification::centred);
    lowCutLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(225, 235, 230));
    lowCutLabel.setFont(juce::Font(13.5f, juce::Font::bold));
    addAndMakeVisible(lowCutLabel);

    peakFreqLabel.setText("FREQ", juce::dontSendNotification);
    peakFreqLabel.setJustificationType(juce::Justification::centred);
    peakFreqLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(225, 235, 230));
    peakFreqLabel.setFont(juce::Font(13.5f, juce::Font::bold));
    addAndMakeVisible(peakFreqLabel);

    peakGainLabel.setText("GAIN", juce::dontSendNotification);
    peakGainLabel.setJustificationType(juce::Justification::centred);
    peakGainLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(225, 235, 230));
    peakGainLabel.setFont(juce::Font(13.5f, juce::Font::bold));
    addAndMakeVisible(peakGainLabel);

    peakQLabel.setText("Q", juce::dontSendNotification);
    peakQLabel.setJustificationType(juce::Justification::centred);
    peakQLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(225, 235, 230));
    peakQLabel.setFont(juce::Font(13.5f, juce::Font::bold));
    addAndMakeVisible(peakQLabel);

    highCutLabel.setText("HIGH CUT", juce::dontSendNotification);
    highCutLabel.setJustificationType(juce::Justification::centred);
    highCutLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(225, 235, 230));
    highCutLabel.setFont(juce::Font(13.5f, juce::Font::bold));
    addAndMakeVisible(highCutLabel);

    setSize(600, 280);
    startTimerHz(30);
}

BlackLabEQAudioProcessorEditor::~BlackLabEQAudioProcessorEditor()
{
}

//==============================================================================
void BlackLabEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    drawBackground(g);
    drawResponseCurve(g, getAnalysisArea());
}

void BlackLabEQAudioProcessorEditor::resized()
{
    const int knobSize = 90;
    const int labelHeight = 20;

    const int lowCutX = 30;
    const int peakFreqX = 145;
    const int peakGainX = 255;
    const int peakQX = 365;
    const int highCutX = 480;

    const int knobY = 145;

    lowCutFreqSlider.setBounds(lowCutX, knobY, knobSize, knobSize);
    peakFreqSlider.setBounds(peakFreqX, knobY, knobSize, knobSize);
    peakGainSlider.setBounds(peakGainX, knobY, knobSize, knobSize);
    peakQualitySlider.setBounds(peakQX, knobY, knobSize, knobSize);
    highCutFreqSlider.setBounds(highCutX, knobY, knobSize, knobSize);

    lowCutLabel.setBounds(lowCutX, knobY + knobSize + 4, knobSize, labelHeight);
    peakFreqLabel.setBounds(peakFreqX, knobY + knobSize + 4, knobSize, labelHeight);
    peakGainLabel.setBounds(peakGainX, knobY + knobSize + 4, knobSize, labelHeight);
    peakQLabel.setBounds(peakQX, knobY + knobSize + 4, knobSize, labelHeight);
    highCutLabel.setBounds(highCutX, knobY + knobSize + 4, knobSize, labelHeight);
}

void BlackLabEQAudioProcessorEditor::timerCallback()
{
    repaint();
}

std::vector<juce::Component*> BlackLabEQAudioProcessorEditor::getComps()
{
    return
    {
        &lowCutFreqSlider,
        &peakFreqSlider,
        &peakGainSlider,
        &peakQualitySlider,
        &highCutFreqSlider
    };
}

juce::Rectangle<int> BlackLabEQAudioProcessorEditor::getRenderArea()
{
    return getLocalBounds().reduced(14);
}

juce::Rectangle<int> BlackLabEQAudioProcessorEditor::getAnalysisArea()
{
    return { 110, 60, 380, 85 };
}

void BlackLabEQAudioProcessorEditor::drawBackground(juce::Graphics& g)
{
    using namespace juce;

    auto background = Colour::fromRGB(72, 130, 122);
    auto panel = Colour::fromRGB(64, 118, 111);
    auto outline = Colour::fromRGBA(255, 255, 255, 35);
    auto text = Colour::fromRGB(228, 236, 232);

    g.fillAll(background);

    auto panelArea = getRenderArea();

    g.setColour(panel);
    g.fillRoundedRectangle(panelArea.toFloat(), 10.0f);

    g.setColour(outline);
    g.drawRoundedRectangle(panelArea.toFloat(), 10.0f, 1.0f);

    g.setColour(Colour::fromRGBA(255, 255, 255, 28));
    g.drawVerticalLine(128.0f, 150.0f, 265.0f);
    g.drawVerticalLine(470.0f, 150.0f, 265.0f);

    g.setColour(text);
    g.setFont(16.0f);
    g.drawText("BLACKLAB EQ", 0, 12, getWidth(), 22, Justification::centred);
}

void BlackLabEQAudioProcessorEditor::drawResponseCurve(juce::Graphics& g, const juce::Rectangle<int>& responseArea)
{
    using namespace juce;

    g.setColour(Colour::fromRGBA(0, 0, 0, 25));
    g.fillRoundedRectangle(responseArea.toFloat(), 6.0f);

    auto settings = getChainSettings(audioProcessor.apvts);

    auto sampleRate = audioProcessor.getSampleRate();
    if (sampleRate <= 0.0)
        sampleRate = 44100.0;

    auto peakCoefficients =
        juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            sampleRate,
            settings.peakFreq,
            settings.peakQuality,
            juce::Decibels::decibelsToGain(settings.peakGainInDb));

    auto lowCutCoefficients =
        juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(
            settings.lowCutFreq,
            sampleRate,
            2 * (settings.lowCutSlope + 1));

    auto highCutCoefficients =
        juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(
            settings.highCutFreq,
            sampleRate,
            2 * (settings.highCutSlope + 1));

    Path responseCurve;
    const auto width = responseArea.getWidth();

    std::vector<double> mags;
    mags.resize(width);

    for (int i = 0; i < width; ++i)
    {
        double mag = 1.0;
        auto freq = mapToLog10((double)i / (double)width, 20.0, 20000.0);

        mag *= peakCoefficients->getMagnitudeForFrequency(freq, sampleRate);

        for (size_t j = 0; j < lowCutCoefficients.size(); ++j)
            mag *= lowCutCoefficients[j]->getMagnitudeForFrequency(freq, sampleRate);

        for (size_t j = 0; j < highCutCoefficients.size(); ++j)
            mag *= highCutCoefficients[j]->getMagnitudeForFrequency(freq, sampleRate);

        mags[i] = Decibels::gainToDecibels(mag);
    }

    auto mapY = [&](double db)
    {
        return jmap(db,
                    -24.0, 24.0,
                    (double)responseArea.getBottom(),
                    (double)responseArea.getY());
    };

    responseCurve.startNewSubPath(responseArea.getX(), mapY(mags.front()));

    for (int i = 1; i < mags.size(); ++i)
    {
        responseCurve.lineTo(responseArea.getX() + i, mapY(mags[i]));
    }

    g.saveState();
    g.reduceClipRegion(responseArea);

    g.setColour(Colour::fromRGBA(120, 220, 200, 80));
    g.strokePath(responseCurve, PathStrokeType(4.0f));

    g.setColour(Colour::fromRGB(120, 220, 200));
    g.strokePath(responseCurve, PathStrokeType(2.0f));

    g.restoreState();

    g.setColour(Colour::fromRGBA(255, 255, 255, 20));
    g.drawRoundedRectangle(responseArea.toFloat(), 6.0f, 1.0f);
}

