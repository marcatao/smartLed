import React,{useRef, useState,useEffect} from 'react';
import {View,Button} from 'react-native'

import sytle from './style';

const BotaoCores = (props) =>{
    
    const changeLed = (val) =>{
        const colors = val.split('.');
        console.log('changed'+colors[0]);
    }

    return(
  
            <Button
                onPress={() => changeLed(props.lightColor)}
                title={props.title}
                color={props.colorButton}
                accessibilityLabel="Learn more about this purple button"
              
            />
    
    )
}

export default BotaoCores;