import React from 'react';
import {Text, View} from 'react-native';


import style from './src/Components/App/style';
import ColorPickerCP from './src/Components/ColorPickerCP';

import BotaoCores from './src/Components/BotaoCores'
const App = () =>  {
  return (
    <View style={style.body}>
      <BotaoCores 
          title="Azul"
          colorButton="#0000FF"
          lightColor="0.0.300"
          />
    </View>
  );
};


export default App;