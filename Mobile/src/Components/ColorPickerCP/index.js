import React,{useRef, useState,useEffect} from 'react';
import { SafeAreaView, StyleSheet, StatusBar, View, Text, TextInput } from 'react-native';
import { ColorPicker} from 'react-native-color-picker-light'
import { IcBrightness } from './ic_brightness';
import Slider from '@react-native-community/slider';


const ColorPickerCP = () =>{
    const [brightness, setBrightness] = useState(100);
    const colorPicker = useRef(null);
    const [valor , setValor] = useState('321');
    const [ip , setIP] = useState('http://192.168.15.1');
    useEffect(() => {
      // setTimeout(() => {
      //   colorPicker.current?.setColor('#f0ce78');
      // }, 1000);
    }, []);

    function hexToRgb(hex) {
        var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
        return result ? {
          r: parseInt(result[1], 16),
          g: parseInt(result[2], 16),
          b: parseInt(result[3], 16)
        } : null;
      }

    function sendColor(color){
        const colorset = hexToRgb(color);
        console.log(colorset);
        setValor(ip+'?r='+colorset.r+'&g='+colorset.g+'&b='+colorset.b)
        const fotosHTTP = fetch(valor);
    }


    return(
        <>
        <StatusBar barStyle="dark-content" />
        <SafeAreaView>
          <View style={styles.appbar}>
            <Text style={styles.appbarTitle}>Click</Text>
          </View>
          <ColorPicker
            ref={colorPicker}
            type="color"
            style={styles.colorPicker}
            onInit={() => {
              console.log('color picker initialized');
              colorPicker.current?.setColor('#f0ce78');
            }}
            onColorChange={color => {
              sendColor(color);
            }}
          />
          <View style={styles.brightnessRoot}>
            <IcBrightness color="#505050" style={styles.brightnessIcon} />
            <Slider
              style={styles.brightnessSlider}
              step={1}
              value={100}
              minimumValue={20}
              maximumValue={100}
              onValueChange={val => setBrightness(val)}
            />
            <Text style={styles.brightnessValue}>{Math.floor(brightness)}</Text>
          </View>
          <View>
              <TextInput value={ip} onChangeText={val => setIP(val)}/>
          </View>
          <View>
              <Text>{valor}</Text>
          </View>
        </SafeAreaView>
      </>
    );
}
const styles = StyleSheet.create({
    appbar: {
      height: 60,
      backgroundColor: '#fff',
      flexDirection: 'row',
      alignItems: 'center',
      justifyContent: 'center',
      paddingHorizontal: 15,
    },
    appbarTitle: {
      fontSize: 30,
      color: '#000',
      fontWeight: 'bold',
      
    },
    colorPicker: {
      aspectRatio: 1,
    },
    brightnessRoot: {
      height: 60,
      marginTop: 20,
      paddingHorizontal: 15,
      backgroundColor: '#fff',
      flexDirection: 'row',
      alignItems: 'center',
    },
    brightnessIcon: {
      height: '50%',
      aspectRatio: 1,
    },
    brightnessSlider: {
      flex: 1,
      marginHorizontal: 15,
    },
    brightnessValue: {
      width: 40,
      fontSize: 20,
      color: '#606060',
      textAlign: 'center',
    },
  });
export default ColorPickerCP;