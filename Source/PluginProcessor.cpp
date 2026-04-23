/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BlackLabEQAudioProcessor::BlackLabEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

BlackLabEQAudioProcessor::~BlackLabEQAudioProcessor()
{
}

//==============================================================================
const juce::String BlackLabEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BlackLabEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BlackLabEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BlackLabEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BlackLabEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BlackLabEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BlackLabEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BlackLabEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BlackLabEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void BlackLabEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BlackLabEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    juce::dsp::ProcessSpec spec;
    
    spec.maximumBlockSize = samplesPerBlock;
    
    spec.numChannels = 1;
    
    spec.sampleRate = sampleRate;
    
    LeftChain.prepare(spec);
    RightChain.prepare(spec);
    
    
    updateFilters();
    
    
    
}

void BlackLabEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BlackLabEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BlackLabEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    updateFilters();
    

    
    juce::dsp::AudioBlock<float> block(buffer);
    
    auto leftBlock = block.getSingleChannelBlock(0);
    
    auto rightBlock = block.getSingleChannelBlock(1);
    
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
    
    LeftChain.process(leftContext);
    RightChain.process(rightContext);
    
    
    
    
    
    
}

//==============================================================================
bool BlackLabEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BlackLabEQAudioProcessor::createEditor()
{
    return new BlackLabEQAudioProcessorEditor (*this);
    
   // return new juce::GenericAudioProcessorEditor(*this);
    
    
}

//==============================================================================
void BlackLabEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    juce::MemoryOutputStream mos(destData,true);
    apvts.state.writeToStream(mos);
    
    
    
}

void BlackLabEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if(tree.isValid())
    {
        apvts.replaceState(tree);
        updateFilters();
    }
}


ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings settings;
    
    settings.lowCutFreq = apvts.getRawParameterValue("LowCutFreq")->load();
    settings.highCutFreq = apvts.getRawParameterValue("HighCutFreq")->load();
    settings.peakFreq = apvts.getRawParameterValue("PeakFreq")->load();
    settings.peakGainInDb = apvts.getRawParameterValue("PeakGain")->load();
    settings.peakQuality = apvts.getRawParameterValue("PeakQuality")->load();
    settings.lowCutSlope = static_cast<Slope>(apvts.getRawParameterValue("LowCutSlope")->load());
    settings.highCutSlope = static_cast<Slope>(apvts.getRawParameterValue("HighCutSlope")->load());
    

    
    return settings;
}

void BlackLabEQAudioProcessor::updatePeakFilter(const ChainSettings& chainSettings)
{
    auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), chainSettings.peakFreq, chainSettings.peakQuality, juce::Decibels::decibelsToGain(chainSettings.peakGainInDb));
    
   // LeftChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;
    //RightChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;
    
    updateCoefficients(LeftChain.get<ChainPositions::Peak>().coefficients, *peakCoefficients);
    updateCoefficients(RightChain.get<ChainPositions::Peak>().coefficients, *peakCoefficients);
    
    
}

void BlackLabEQAudioProcessor::updateCoefficients(Coefficients &old, const Coefficients &replacements)
{
    *old = *replacements;
    
}


void BlackLabEQAudioProcessor::updateLowCutFilters(const ChainSettings& chainSettings)
{
    auto lowCutCoefficients =
        juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(
            chainSettings.lowCutFreq,
            getSampleRate(),
            2 * (chainSettings.lowCutSlope + 1));

    updateCutFilter(LeftChain.get<LowCut>(), lowCutCoefficients, chainSettings.lowCutSlope);
    updateCutFilter(RightChain.get<LowCut>(), lowCutCoefficients, chainSettings.lowCutSlope);
}

void BlackLabEQAudioProcessor::updateHighCutFilters(const ChainSettings& chainSettings)
{
    auto highCutCoefficients =
        juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(
            chainSettings.highCutFreq,
            getSampleRate(),
            2 * (chainSettings.highCutSlope + 1));

    updateCutFilter(LeftChain.get<HighCut>(), highCutCoefficients, chainSettings.highCutSlope);
    updateCutFilter(RightChain.get<HighCut>(), highCutCoefficients, chainSettings.highCutSlope);
}

void BlackLabEQAudioProcessor::updateFilters()
{
    auto chainSettings = getChainSettings(apvts);

    updatePeakFilter(chainSettings);
    updateLowCutFilters(chainSettings);
    updateHighCutFilters(chainSettings);
}



 juce::AudioProcessorValueTreeState::ParameterLayout BlackLabEQAudioProcessor::createParameterLayout()
{
     juce::AudioProcessorValueTreeState::ParameterLayout layout;
     
     layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"LowCutFreq",1},
                                                            "LowCut Freq",
                                                            juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 20.f));
     
  
     layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"HighCutFreq",1},
                                                            
                                                            "HighCut Freq",
                                                            juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 20000.f));
     

     layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"PeakFreq",1},
                                                            
                                                            "PeakFreq",
                                                            juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f), 750.f));
     
     layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"PeakGain",1},
                                                            
                                                            "PeakGain",
                                                            juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 0.25f), 0.0f));
     
     layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"PeakQuality",1},
                                                            
                                                            "PeakQUality",
                                                            juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 0.25), 1.f));
     

     juce::StringArray stringArray;
     for(int i = 0; i < 4; i++)
     {
         juce::String str;
         str << (12 + i*12);
         str << "db/oct";
         stringArray.add(str);
     }
     
     layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"LowCutSlope",1}, "LowCutSlope", stringArray, 0));
     
     layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"HighCutSlope",1}, "HighCutSlope", stringArray, 0));
     
     
     
     return layout;
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BlackLabEQAudioProcessor();
}
